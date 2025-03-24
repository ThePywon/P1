use std::any::TypeId;

use thiserror::Error;

#[derive(Error, Debug)]
pub enum EventError {
  #[error("The provided EventData for registration is already registered.")]
  EventAlreadyRegistered(TypeId),
  #[error("No event listeners for the provided EventData was found.")]
  EventNotFound(TypeId)
}
