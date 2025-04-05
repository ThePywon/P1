use crate::event::EventManager;

use std::sync::Arc;

use parking_lot::RwLock;
use winit::application::ApplicationHandler;
use winit::event_loop::{ActiveEventLoop, ControlFlow, EventLoop};
use winit::window::{Window, WindowId};
use winit::event::WindowEvent;

struct WindowHandler {
  event_manager: Arc<RwLock<EventManager>>,
  window: Option<Window>
}

impl ApplicationHandler for WindowHandler {
  fn resumed(&mut self, event_loop: &ActiveEventLoop) {
    self.window = Some(event_loop.create_window(Window::default_attributes()).unwrap());
    // Do an event here
  }

  fn window_event(&mut self, event_loop: &ActiveEventLoop, window_id: WindowId, event: WindowEvent) {
    match event {
      WindowEvent::CloseRequested => {
        event_loop.exit();
        // Do an event here maybe?
      },
      WindowEvent::RedrawRequested => {
        // TODO
        // Cameras will need to handle events here as well
      },
      WindowEvent::KeyboardInput { device_id, event, is_synthetic } => {
        // Do an event here
      },
      _ => {}
    }
  }
}
