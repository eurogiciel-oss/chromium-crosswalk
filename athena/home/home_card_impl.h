// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ATHENA_HOME_HOME_CARD_IMPL_H_
#define ATHENA_HOME_HOME_CARD_IMPL_H_

#include "athena/athena_export.h"
#include "athena/home/home_card_gesture_manager.h"
#include "athena/home/public/home_card.h"
#include "athena/home/public/search_controller_factory.h"
#include "athena/input/public/accelerator_manager.h"
#include "athena/wm/public/window_manager_observer.h"

namespace aura {
class Window;
}

namespace gfx {
class Rect;
}

namespace ui {
class LayerOwner;
}

namespace views {
class Widget;
}

namespace athena {
class AppModelBuilder;
class AppListViewDelegate;
class HomeCardLayoutManager;
class HomeCardView;

class ATHENA_EXPORT HomeCardImpl : public HomeCard,
                                   public AcceleratorHandler,
                                   public HomeCardGestureManager::Delegate,
                                   public WindowManagerObserver {
 public:
  HomeCardImpl(scoped_ptr<AppModelBuilder> model_builder,
               scoped_ptr<SearchControllerFactory> search_factory);
  virtual ~HomeCardImpl();

  void Init();

  aura::Window* GetHomeCardWindowForTest() const;

 private:
  enum Command {
    COMMAND_SHOW_HOME_CARD,
  };
  void InstallAccelerators();

  // Overridden from HomeCard:
  virtual void SetState(HomeCard::State state) override;
  virtual State GetState() override;
  virtual void UpdateVirtualKeyboardBounds(
      const gfx::Rect& bounds) override;

  // AcceleratorHandler:
  virtual bool IsCommandEnabled(int command_id) const override;
  virtual bool OnAcceleratorFired(int command_id,
                                  const ui::Accelerator& accelerator) override;

  // HomeCardGestureManager::Delegate:
  virtual void OnGestureEnded(HomeCard::State final_state,
                              bool is_fling) override;
  virtual void OnGestureProgressed(
      HomeCard::State from_state,
      HomeCard::State to_state,
      float progress) override;

  // WindowManagerObserver:
  virtual void OnOverviewModeEnter() override;
  virtual void OnOverviewModeExit() override;
  virtual void OnSplitViewModeEnter() override;
  virtual void OnSplitViewModeExit() override;

  scoped_ptr<AppModelBuilder> model_builder_;
  scoped_ptr<SearchControllerFactory> search_factory_;

  HomeCard::State state_;

  // original_state_ is the state which the home card should go back to after
  // the virtual keyboard is hidden.
  HomeCard::State original_state_;

  views::Widget* home_card_widget_;
  HomeCardView* home_card_view_;
  scoped_ptr<AppListViewDelegate> view_delegate_;
  HomeCardLayoutManager* layout_manager_;
  scoped_ptr<ui::LayerOwner> minimized_home_;

  DISALLOW_COPY_AND_ASSIGN(HomeCardImpl);
};

}  // namespace athena

#endif  // ATHENA_HOME_HOME_CARD_IMPL_H_
