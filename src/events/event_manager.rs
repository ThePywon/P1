use std::collections::HashMap;
use std::hash::Hash;

pub struct EventManager<K: Eq + Hash, T = ()> {
  events: HashMap<K, Vec<fn(&T)>>
}

impl<K: Eq + Hash, T> EventManager<K, T> {
  pub fn new() -> Self {
    Self::default()
  }

  pub fn on(&mut self, key: K, func: fn(&T)) {
    match self.events.get_mut(&key) {
      Some(callbacks) => callbacks.push(func),
      None => { self.events.insert(key, vec![func]); }
    }
  }

  pub fn emit(&self, key: K, arg: T) {
    if let Some(callbacks) = self.events.get(&key) {
      for callback in callbacks.iter() {
        callback(&arg);
      }
    }
  }
}

impl<K: Eq + Hash, T> Default for EventManager<K, T> {
  fn default() -> Self {
    EventManager { events: HashMap::new() }
  }
}
