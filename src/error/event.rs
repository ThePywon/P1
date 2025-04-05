use crate::event::Tick;

use std::any::TypeId;

use thiserror::Error;

#[derive(Error, Debug)]
pub enum EventError {
  #[error("The provided EventData for registration is already registered.")]
  EventAlreadyRegistered(TypeId),
  #[error("No event listeners for the provided EventData was found, consider registering it beforehand.")]
  EventNotFound(TypeId),
  #[error("An event with associated data was emitted without data and could not be fetched.")]
  CannotFetchEventData(TypeId, Tick)
}
