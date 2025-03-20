use super::tick::Tick;

use std::cmp::Ordering;
use std::sync::Arc;
use std::ops::Add;
use std::any::Any;

use parking_lot::RwLock;


pub trait Event: Send + Sync + Any {
  fn check(&self, other: &Tick) -> bool;
}

pub struct SimpleEvent(Arc<RwLock<Tick>>);

impl SimpleEvent {
  pub fn new() -> Self {
    Self( Arc::new(RwLock::new(Tick::new())) )
  }

  pub fn emit(&self) {
    self.0.write().touch();
  }
}
impl Event for SimpleEvent {
  fn check(&self, other: &Tick) -> bool {
    self.0.read().cmp(other) == Ordering::Greater
  }
}

pub struct IntervalEvent(Arc<RwLock<Tick>>, u32);

impl IntervalEvent {
  pub fn new(interval: u32) -> Self {
    Self( Arc::new(RwLock::new(Tick::new())), interval )
  }

  fn update(&self) {
    let check = self.0.read().add(self.1).cmp(&Tick::new());
    if check == Ordering::Less || check == Ordering::Equal {
      self.0.write().touch();
    }
  }
}
impl Event for IntervalEvent {
  fn check(&self, other: &Tick) -> bool {
    self.update();

    self.0.read().cmp(other) == Ordering::Greater
  }
}
