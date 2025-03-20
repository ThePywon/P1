use std::ops::{Add, AddAssign};

use chrono::{DateTime, TimeDelta, Utc};

#[derive(PartialEq, Eq, PartialOrd, Ord, Clone, Copy)]
pub struct Tick(DateTime<Utc>);

impl Tick {
  pub fn new() -> Self {
    Self( Utc::now() )
  }

  pub fn touch(&mut self) {
    self.0 = Utc::now()
  }
}

impl From<DateTime<Utc>> for Tick {
  fn from(value: DateTime<Utc>) -> Self {
    Self( value )
  }
}

impl Add<u32> for Tick {
  type Output = Self;

  fn add(self, rhs: u32) -> Self::Output {
    self.0.checked_add_signed(TimeDelta::milliseconds(rhs.into())).unwrap().into()
  }
}
impl AddAssign<u32> for Tick {
  fn add_assign(&mut self, rhs: u32) {
    self.0 = self.0.checked_add_signed(TimeDelta::milliseconds(rhs.into())).unwrap()
  }
}
