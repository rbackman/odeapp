// generated by Fast Light User Interface Designer (fluid) version 2.1000

#ifndef app_fluid_h
#define app_fluid_h
# include "gsim/fl_viewer.h"
# include "app_events.h"
# include "app_viewer.h"
#include <fltk/Window.h>
#include <fltk/TiledGroup.h>
#include <fltk/Group.h>
#include <fltk/InvisibleBox.h>
#include <fltk/MenuBar.h>
#include <fltk/Item.h>
#include <fltk/ItemGroup.h>
#include <fltk/Divider.h>
#include <fltk/TabGroup.h>
#include <fltk/CheckButton.h>
#include <fltk/ValueInput.h>
#include <fltk/Dial.h>
#include <fltk/LightButton.h>
#include <fltk/ValueSlider.h>
#include <fltk/Button.h>
#include <fltk/MultiBrowser.h>
#include <fltk/Input.h>

class AppFluid  {
public:
  virtual void event ( AppEvent e )=0;
  AppFluid();
  fltk::Window *ui_window;
      AppViewer *ui_viewer;
      fltk::Group *ui_main_group;
private:
          inline void cb_X_i(fltk::Item*, void*);
          static void cb_X(fltk::Item*, void*);
            inline void cb_parmeters_i(fltk::Item*, void*);
            static void cb_parmeters(fltk::Item*, void*);
            inline void cb_double_i(fltk::Item*, void*);
            static void cb_double(fltk::Item*, void*);
            inline void cb_left_i(fltk::Item*, void*);
            static void cb_left(fltk::Item*, void*);
            inline void cb_right_i(fltk::Item*, void*);
            static void cb_right(fltk::Item*, void*);
            inline void cb_Load_i(fltk::Item*, void*);
            static void cb_Load(fltk::Item*, void*);
            inline void cb_Fun_i(fltk::Item*, void*);
            static void cb_Fun(fltk::Item*, void*);
public:
          fltk::TabGroup *ui_main_tab;
                fltk::CheckButton *ui_useComJ;
private:
                inline void cb_ui_useComJ_i(fltk::CheckButton*, void*);
                static void cb_ui_useComJ(fltk::CheckButton*, void*);
public:
                fltk::ValueInput *ui_jkp;
private:
                inline void cb_ui_jkp_i(fltk::ValueInput*, void*);
                static void cb_ui_jkp(fltk::ValueInput*, void*);
public:
                fltk::ValueInput *ui_jkd;
private:
                inline void cb_ui_jkd_i(fltk::ValueInput*, void*);
                static void cb_ui_jkd(fltk::ValueInput*, void*);
public:
                fltk::ValueInput *ui_jkpc;
private:
                inline void cb_ui_jkpc_i(fltk::ValueInput*, void*);
                static void cb_ui_jkpc(fltk::ValueInput*, void*);
public:
                fltk::ValueInput *ui_jkdc;
private:
                inline void cb_ui_jkdc_i(fltk::ValueInput*, void*);
                static void cb_ui_jkdc(fltk::ValueInput*, void*);
public:
                fltk::ValueInput *ui_vs;
private:
                inline void cb_ui_vs_i(fltk::ValueInput*, void*);
                static void cb_ui_vs(fltk::ValueInput*, void*);
public:
                fltk::ValueInput *ui_vc;
private:
                inline void cb_ui_vc_i(fltk::ValueInput*, void*);
                static void cb_ui_vc(fltk::ValueInput*, void*);
public:
                fltk::CheckButton *ui_useBubbleT;
private:
                inline void cb_ui_useBubbleT_i(fltk::CheckButton*, void*);
                static void cb_ui_useBubbleT(fltk::CheckButton*, void*);
public:
                fltk::Dial *ui_walk_dir;
private:
                inline void cb_ui_walk_dir_i(fltk::Dial*, void*);
                static void cb_ui_walk_dir(fltk::Dial*, void*);
public:
                fltk::LightButton *ui_walk;
private:
                inline void cb_ui_walk_i(fltk::LightButton*, void*);
                static void cb_ui_walk(fltk::LightButton*, void*);
public:
                fltk::ValueSlider *ui_walk_speed;
private:
                inline void cb_ui_walk_speed_i(fltk::ValueSlider*, void*);
                static void cb_ui_walk_speed(fltk::ValueSlider*, void*);
public:
                fltk::CheckButton *ui_arrow;
private:
                inline void cb_ui_arrow_i(fltk::CheckButton*, void*);
                static void cb_ui_arrow(fltk::CheckButton*, void*);
public:
                fltk::ValueSlider *ui_step_dist;
private:
                inline void cb_ui_step_dist_i(fltk::ValueSlider*, void*);
                static void cb_ui_step_dist(fltk::ValueSlider*, void*);
public:
                fltk::ValueSlider *ui_step_height;
private:
                inline void cb_ui_step_height_i(fltk::ValueSlider*, void*);
                static void cb_ui_step_height(fltk::ValueSlider*, void*);
public:
                fltk::ValueSlider *ui_step_speed;
private:
                inline void cb_ui_step_speed_i(fltk::ValueSlider*, void*);
                static void cb_ui_step_speed(fltk::ValueSlider*, void*);
public:
                fltk::Group *ui_message;
                fltk::ValueSlider *ui_walk_sway;
private:
                inline void cb_ui_walk_sway_i(fltk::ValueSlider*, void*);
                static void cb_ui_walk_sway(fltk::ValueSlider*, void*);
public:
                fltk::ValueSlider *ui_walk_hheight;
private:
                inline void cb_ui_walk_hheight_i(fltk::ValueSlider*, void*);
                static void cb_ui_walk_hheight(fltk::ValueSlider*, void*);
                inline void cb_Step_i(fltk::Button*, void*);
                static void cb_Step(fltk::Button*, void*);
public:
                fltk::CheckButton *ui_useGravC;
private:
                inline void cb_ui_useGravC_i(fltk::CheckButton*, void*);
                static void cb_ui_useGravC(fltk::CheckButton*, void*);
public:
                fltk::CheckButton *ui_usePID;
private:
                inline void cb_ui_usePID_i(fltk::CheckButton*, void*);
                static void cb_ui_usePID(fltk::CheckButton*, void*);
                inline void cb_rest_i(fltk::Button*, void*);
                static void cb_rest(fltk::Button*, void*);
public:
                fltk::ValueInput *ui_kg;
private:
                inline void cb_ui_kg_i(fltk::ValueInput*, void*);
                static void cb_ui_kg(fltk::ValueInput*, void*);
public:
                fltk::CheckButton *ui_collide_feet_only;
private:
                inline void cb_ui_collide_feet_only_i(fltk::CheckButton*, void*);
                static void cb_ui_collide_feet_only(fltk::CheckButton*, void*);
public:
                fltk::CheckButton *ui_root_manip;
private:
                inline void cb_ui_root_manip_i(fltk::CheckButton*, void*);
                static void cb_ui_root_manip(fltk::CheckButton*, void*);
public:
                fltk::Group *ui_stepState;
private:
                inline void cb_Left_i(fltk::Button*, void*);
                static void cb_Left(fltk::Button*, void*);
                inline void cb_Right_i(fltk::Button*, void*);
                static void cb_Right(fltk::Button*, void*);
                inline void cb_Both_i(fltk::Button*, void*);
                static void cb_Both(fltk::Button*, void*);
                inline void cb_Toes_i(fltk::Button*, void*);
                static void cb_Toes(fltk::Button*, void*);
                inline void cb_Heel_i(fltk::Button*, void*);
                static void cb_Heel(fltk::Button*, void*);
                inline void cb_Fixed_i(fltk::Button*, void*);
                static void cb_Fixed(fltk::Button*, void*);
public:
                fltk::CheckButton *ui_contactVis;
private:
                inline void cb_ui_contactVis_i(fltk::CheckButton*, void*);
                static void cb_ui_contactVis(fltk::CheckButton*, void*);
public:
                fltk::MultiBrowser *ui_config_browser;
private:
                inline void cb_ui_config_browser_i(fltk::MultiBrowser*, void*);
                static void cb_ui_config_browser(fltk::MultiBrowser*, void*);
                inline void cb_save_i(fltk::Button*, void*);
                static void cb_save(fltk::Button*, void*);
                inline void cb_update_i(fltk::Button*, void*);
                static void cb_update(fltk::Button*, void*);
                inline void cb_load_i(fltk::Button*, void*);
                static void cb_load(fltk::Button*, void*);
                inline void cb_delete_i(fltk::Button*, void*);
                static void cb_delete(fltk::Button*, void*);
public:
                fltk::Input *ui_current_parm;
                fltk::CheckButton *ui_use_root;
private:
                inline void cb_ui_use_root_i(fltk::CheckButton*, void*);
                static void cb_ui_use_root(fltk::CheckButton*, void*);
public:
                fltk::ValueInput *ui_root_kp;
private:
                inline void cb_ui_root_kp_i(fltk::ValueInput*, void*);
                static void cb_ui_root_kp(fltk::ValueInput*, void*);
public:
                fltk::ValueInput *ui_root_kd;
private:
                inline void cb_ui_root_kd_i(fltk::ValueInput*, void*);
                static void cb_ui_root_kd(fltk::ValueInput*, void*);
public:
                fltk::ValueInput *ui_root_maxV;
private:
                inline void cb_ui_root_maxV_i(fltk::ValueInput*, void*);
                static void cb_ui_root_maxV(fltk::ValueInput*, void*);
public:
                fltk::ValueInput *ui_root_damp;
private:
                inline void cb_ui_root_damp_i(fltk::ValueInput*, void*);
                static void cb_ui_root_damp(fltk::ValueInput*, void*);
              inline void cb_ui_jnt_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt(fltk::ValueSlider*, void*);
              inline void cb_ui_jnt1_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt1(fltk::ValueSlider*, void*);
              inline void cb_ui_jnt2_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt2(fltk::ValueSlider*, void*);
              inline void cb_ui_jnt3_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt3(fltk::ValueSlider*, void*);
              inline void cb_ui_jnt4_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt4(fltk::ValueSlider*, void*);
              inline void cb_ui_jnt5_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt5(fltk::ValueSlider*, void*);
              inline void cb_ui_jnt6_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt6(fltk::ValueSlider*, void*);
              inline void cb_ui_jnt7_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt7(fltk::ValueSlider*, void*);
              inline void cb_ui_jnt8_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt8(fltk::ValueSlider*, void*);
              inline void cb_ui_jnt9_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt9(fltk::ValueSlider*, void*);
              inline void cb_ui_jnta_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnta(fltk::ValueSlider*, void*);
              inline void cb_ui_jntb_i(fltk::ValueSlider*, void*);
              static void cb_ui_jntb(fltk::ValueSlider*, void*);
              inline void cb_ui_jntc_i(fltk::ValueSlider*, void*);
              static void cb_ui_jntc(fltk::ValueSlider*, void*);
              inline void cb_ui_jntd_i(fltk::ValueSlider*, void*);
              static void cb_ui_jntd(fltk::ValueSlider*, void*);
              inline void cb_ui_jnte_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnte(fltk::ValueSlider*, void*);
              inline void cb_ui_jntf_i(fltk::ValueSlider*, void*);
              static void cb_ui_jntf(fltk::ValueSlider*, void*);
              inline void cb_ui_jnt10_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt10(fltk::ValueSlider*, void*);
              inline void cb_ui_jnt11_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt11(fltk::ValueSlider*, void*);
              inline void cb_ui_jnt12_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt12(fltk::ValueSlider*, void*);
              inline void cb_ui_jnt13_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt13(fltk::ValueSlider*, void*);
public:
              fltk::ValueSlider *ui_jnt[21];
private:
              inline void cb_ui_jnt14_i(fltk::ValueSlider*, void*);
              static void cb_ui_jnt14(fltk::ValueSlider*, void*);
public:
              fltk::CheckButton *ui_showSkel;
private:
              inline void cb_ui_showSkel_i(fltk::CheckButton*, void*);
              static void cb_ui_showSkel(fltk::CheckButton*, void*);
public:
              fltk::CheckButton *ui_shaded;
private:
              inline void cb_ui_shaded_i(fltk::CheckButton*, void*);
              static void cb_ui_shaded(fltk::CheckButton*, void*);
public:
              fltk::MultiBrowser *ui_jointList;
private:
              inline void cb_ui_jointList_i(fltk::MultiBrowser*, void*);
              static void cb_ui_jointList(fltk::MultiBrowser*, void*);
public:
              fltk::ValueSlider *ui_jointP;
private:
              inline void cb_ui_jointP_i(fltk::ValueSlider*, void*);
              static void cb_ui_jointP(fltk::ValueSlider*, void*);
public:
              fltk::ValueSlider *ui_jointD;
private:
              inline void cb_ui_jointD_i(fltk::ValueSlider*, void*);
              static void cb_ui_jointD(fltk::ValueSlider*, void*);
public:
              fltk::CheckButton *ui_jointGrav;
private:
              inline void cb_ui_jointGrav_i(fltk::CheckButton*, void*);
              static void cb_ui_jointGrav(fltk::CheckButton*, void*);
public:
              fltk::CheckButton *ui_jointPD;
private:
              inline void cb_ui_jointPD_i(fltk::CheckButton*, void*);
              static void cb_ui_jointPD(fltk::CheckButton*, void*);
              inline void cb_Save_i(fltk::Button*, void*);
              static void cb_Save(fltk::Button*, void*);
              inline void cb_Load1_i(fltk::Button*, void*);
              static void cb_Load1(fltk::Button*, void*);
public:
              fltk::CheckButton *ui_jointVf;
private:
              inline void cb_ui_jointVf_i(fltk::CheckButton*, void*);
              static void cb_ui_jointVf(fltk::CheckButton*, void*);
public:
              fltk::ValueSlider *ui_jointJX;
private:
              inline void cb_ui_jointJX_i(fltk::ValueSlider*, void*);
              static void cb_ui_jointJX(fltk::ValueSlider*, void*);
public:
              fltk::ValueSlider *ui_jointJY;
private:
              inline void cb_ui_jointJY_i(fltk::ValueSlider*, void*);
              static void cb_ui_jointJY(fltk::ValueSlider*, void*);
public:
              fltk::ValueSlider *ui_jointJZ;
private:
              inline void cb_ui_jointJZ_i(fltk::ValueSlider*, void*);
              static void cb_ui_jointJZ(fltk::ValueSlider*, void*);
public:
              fltk::Group *ui_multiSelect;
        fltk::LightButton *ui_ode_run;
private:
        inline void cb_ui_ode_run_i(fltk::LightButton*, void*);
        static void cb_ui_ode_run(fltk::LightButton*, void*);
        inline void cb_Reset_i(fltk::Button*, void*);
        static void cb_Reset(fltk::Button*, void*);
};
#endif
