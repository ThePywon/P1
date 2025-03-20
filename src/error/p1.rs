use thiserror::Error;

use super::{DataError, InternalDataError, SystemError};

#[derive(Error, Debug)]
pub enum P1Error {
  #[error(transparent)]
  Data(#[from] DataError),
  #[error(transparent)]
  System(#[from] SystemError)
}

impl From<InternalDataError> for P1Error {
  fn from(value: InternalDataError) -> Self {
    P1Error::Data(DataError::Internal(value))
  }
}
