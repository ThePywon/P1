pub mod builtin;
mod event;
mod input;
mod tick;

pub use event::{Event, EventData, EventListener, EventManager, IntervalListener, SimpleListener};
pub use tick::Tick;

#[cfg(test)]
mod tests {
  use super::*;
  use std::cmp::Ordering;
  use std::thread::sleep;
  use std::time::Duration;

  #[test]
  fn compare_ticks() {
    let mut a = Tick::new();
    let mut b = a.clone();

    assert!(a.cmp(&b) == Ordering::Equal);
    assert!(b.cmp(&a) == Ordering::Equal);

    sleep(Duration::from_secs(1));

    a.touch();

    assert!(a.cmp(&b) == Ordering::Greater);
    assert!(b.cmp(&a) == Ordering::Less);

    sleep(Duration::from_secs(1));

    b.touch();

    assert!(b.cmp(&a) == Ordering::Greater);
    assert!(a.cmp(&b) == Ordering::Less);
  }

  // Need more operators for ticks to test with
}
