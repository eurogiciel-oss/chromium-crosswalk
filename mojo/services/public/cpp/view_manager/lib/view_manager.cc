// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mojo/services/public/cpp/view_manager/view_manager.h"

#include "base/bind.h"
#include "base/run_loop.h"
#include "mojo/public/cpp/application/application.h"
#include "mojo/services/public/cpp/view_manager/lib/view_manager_private.h"
#include "mojo/services/public/cpp/view_manager/lib/view_manager_synchronizer.h"
#include "mojo/services/public/cpp/view_manager/lib/view_tree_node_private.h"
#include "mojo/services/public/cpp/view_manager/view.h"

namespace mojo {
namespace view_manager {
namespace {

void OnViewManagerReady(base::RunLoop* loop,
                        ViewManager* manager,
                        ViewTreeNode* root) {
  loop->Quit();
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////
// ViewManager, public:

ViewManager::~ViewManager() {
  while (!nodes_.empty()) {
    IdToNodeMap::iterator it = nodes_.begin();
    if (synchronizer_->OwnsNode(it->second->id()))
      it->second->Destroy();
    else
      nodes_.erase(it);
  }
  while (!views_.empty()) {
    IdToViewMap::iterator it = views_.begin();
    if (synchronizer_->OwnsView(it->second->id()))
      it->second->Destroy();
    else
      views_.erase(it);
  }
}

// static
ViewManager* ViewManager::CreateBlocking(Application* application) {
  base::RunLoop init_loop;
  ViewManager* manager = new ViewManager(
      application,
      base::Bind(&OnViewManagerReady, &init_loop),
      RootCallback());
  init_loop.Run();
  return manager;
}

// static
void ViewManager::Create(
    Application* application,
    const RootCallback& root_added_callback,
    const RootCallback& root_removed_callback) {
  new ViewManager(application, root_added_callback, root_removed_callback);
}

ViewTreeNode* ViewManager::GetNodeById(TransportNodeId id) {
  IdToNodeMap::const_iterator it = nodes_.find(id);
  return it != nodes_.end() ? it->second : NULL;
}

View* ViewManager::GetViewById(TransportViewId id) {
  IdToViewMap::const_iterator it = views_.find(id);
  return it != views_.end() ? it->second : NULL;
}

////////////////////////////////////////////////////////////////////////////////
// ViewManager, private:

ViewManager::ViewManager(
    Application* application,
    const RootCallback& root_added_callback,
    const RootCallback& root_removed_callback)
    : root_added_callback_(root_added_callback),
      root_removed_callback_(root_removed_callback),
      synchronizer_(NULL) {
  application->AddService<ViewManagerSynchronizer>(this);
}

}  // namespace view_manager
}  // namespace mojo
