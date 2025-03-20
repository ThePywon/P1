use std::any::TypeId;
use thiserror::Error;

#[derive(Error, Debug)]
pub enum UtilityContainerError {
  #[error("A type was pushed to an incompatible container.")]
  MismatchedTypeId(TypeId, TypeId),
  #[error("Container for provided component type was not found.")]
  EntryOccupied
}
