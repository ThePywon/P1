use super::EventData;

use chrono::TimeDelta;

pub struct Start;
impl EventData for Start {
  type Item = TimeDelta;
  
  fn get_item(delta: TimeDelta/* , event_managers: _ */) -> Self::Item { delta }
}

pub struct Update;
impl EventData for Update {
  type Item = TimeDelta;

  fn get_item(delta: TimeDelta/* , event_managers: _ */) -> Self::Item { delta }
}
