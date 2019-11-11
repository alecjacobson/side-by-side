#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>

int main(int argc, char *argv[])
{
  int obj_left_id,obj_right_id;
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  igl::read_triangle_mesh(argv[1],V,F);
  // Plot the mesh
  igl::opengl::glfw::Viewer vr;
  unsigned int left_view, right_view;
  // Initialize two views
  vr.core().viewport = Eigen::Vector4f(640, 0, 640, 800);
  right_view = vr.core_list[0].id;
  left_view = vr.append_core(Eigen::Vector4f(0, 0, 640, 800));
  vr.callback_post_resize = 
    [&](igl::opengl::glfw::Viewer &v, int w, int h) 
  {
    v.core( left_view).viewport = Eigen::Vector4f(0, 0, w / 2, h);
    v.core(right_view).viewport = Eigen::Vector4f(w / 2, 0, w - (w / 2), h);
    return true;
  };
  obj_left_id = vr.append_mesh();
  obj_right_id = vr.append_mesh();
  for(int obj_id : {obj_left_id,obj_right_id})
  {
    vr.data_list[obj_id].set_mesh(V, F);
    vr.data_list[obj_id].show_lines = false;
  }
  vr.data_list[obj_left_id].set_colors(Eigen::RowVector3d(0,1,1));
  vr.data_list[obj_right_id].set_colors(Eigen::RowVector3d(1,0,1));
  vr.data_list[obj_left_id].is_visible = left_view;
  vr.data_list[obj_right_id].is_visible = right_view;
  vr.selected_data_index = obj_left_id;

  vr.core(left_view).background_color = Eigen::Vector4f(1,0,0,0);
  vr.core(right_view).background_color = Eigen::Vector4f(0,1,0,0);
  
  vr.callback_mouse_down = [&](igl::opengl::glfw::Viewer &, int button, int mod)->bool
  {
    vr.select_hovered_core();
    const bool on_left_view = vr.core_index(left_view) == vr.selected_core_index;
    //igl::project(
    //  obj,vr.core().view,vr.core().proj, vr.core().viewport,down_win);
    return false;
  };
  vr.callback_mouse_move = [&](igl::opengl::glfw::Viewer &, int x, int y)->bool
  {
    // do _not_ call v.select_hovered_core() here...
    const bool on_left_view = vr.core_index(left_view) == vr.selected_core_index;
    return false;
  };
  vr.callback_mouse_up = [&](igl::opengl::glfw::Viewer &, int button, int mod)->bool
  {
    return false;
  };

  vr.launch_init(true,false,"🙏",1280,720);
  vr.launch_rendering(true);
  vr.launch_shut();
}
