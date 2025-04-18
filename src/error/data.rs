use super::utility::UtilityContainerError;
use thiserror::Error;

#[derive(Error, Debug)]
pub enum InternalDataError {
  #[error("A component was pushed to an incompatible container.")]
  MismatchedComponentType,
  #[error("No container for the provided component type was found.")]
  ContainerNotFound,
}

#[derive(Error, Debug)]
pub enum DataError {
  #[error("No entities with the provided id was found.")]
  EntityNotFound,
  // Need input-defined typename info soon
  #[error(
    "Cannot attach component to entity because a component of that type is already attached."
  )]
  ComponentExistsForEntity,
  #[error(transparent)]
  Internal(#[from] InternalDataError),
}

impl From<UtilityContainerError> for DataError {
  fn from(value: UtilityContainerError) -> Self {
    match value {
      UtilityContainerError::MismatchedTypeId(_, _) => {
        DataError::Internal(InternalDataError::MismatchedComponentType)
      }
      UtilityContainerError::EntryOccupied => DataError::ComponentExistsForEntity,
    }
  }
}
