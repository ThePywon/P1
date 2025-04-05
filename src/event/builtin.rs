use std::any::TypeId;

use crate::error::EventError;
use super::{EventData, EventManager, IntervalListener, SimpleListener, Tick};

use chrono::TimeDelta;
use macros::EventData;
use winit::window::WindowId;

/*pub struct Exit;
impl EventData for Exit {
  type Item = WindowId;

  fn get_item(tick: &Tick/* , command_managers: _ */) -> Self::Item {
    // Will first fetch the command manager using the Self::Item TypeId
    // then the Self TypeId to get the appropriate command list
    // finally, gets the youngest command of that type IF that tick is younger than the passed tick
    command_managers.get_latest_value_for::<Self>(tick).ok_or(EventError::CannotFetchEventData(TypeId::of::<Self>(), *tick))
  }
}*/

#[derive(EventData)]
pub struct Resume;

#[derive(EventData)]
pub struct Update;

// Make this a bitmask
/*pub struct BuiltinSettings {
  pub update: (bool, u32)
}
impl BuiltinSettings {
  pub fn empty() -> Self {
    Self { update: (false, 0u32) }
  }

  pub fn populate_event_manager(&self, event_manager: &mut EventManager) -> Result<(), EventError> {
    if self.update.0 {
      event_manager.register_listener::<Update, _>(IntervalListener::new(self.update.1))?;
    }

    Ok(())
  }
}
impl Default for BuiltinSettings {
  fn default() -> Self {
    Self { update: (true, 0u32) }
  }
}*/
