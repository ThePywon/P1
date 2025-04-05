use std::ops::{Add, AddAssign, Sub, SubAssign};

use chrono::{DateTime, TimeDelta, Utc};

#[derive(PartialEq, Eq, PartialOrd, Ord, Clone, Copy, Debug)]
pub struct Tick(DateTime<Utc>);

impl Tick {
  pub fn new() -> Self {
    Self( Utc::now() )
  }

  pub fn touch(&mut self) {
    self.0 = Utc::now()
  }

  pub fn delta(&self, other: &Tick) -> TimeDelta {
    (self.0 - other.0).abs()
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
impl Sub<u32> for Tick {
  type Output = Self;

  fn sub(self, rhs: u32) -> Self::Output {
    self.0.checked_sub_signed(TimeDelta::milliseconds(rhs.into())).unwrap().into()
  }
}
impl SubAssign<u32> for Tick {
  fn sub_assign(&mut self, rhs: u32) {
    self.0 = self.0.checked_sub_signed(TimeDelta::milliseconds(rhs.into())).unwrap()
  }
}
