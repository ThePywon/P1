use thiserror::Error;

#[derive(Error, Debug)]
pub enum InternalDataError {
  #[error("A component was pushed to an incompatible container.")]
  MismatchedComponentType,
  #[error("Container for provided component type was not found.")]
  ContainerNotFound
}

#[derive(Error, Debug)]
pub enum DataError {
  #[error("No entities with the provided id was found.")]
  EntityNotFound,
  // Need input-defined typename info soon
  #[error("Cannot attach component to entity because a component of that type is already attached.")]
  ComponentExistsForEntity,
  #[error(transparent)]
  Internal(#[from] InternalDataError)
}

#[derive(Error, Debug)]
pub enum SystemError {
  #[error("Not all query items in system were unique.")]
  QueryDeadlock
}

#[derive(Error, Debug)]
pub enum P1Error {
  #[error(transparent)]
  Data(#[from] DataError),
  #[error(transparent)]
  System(#[from] SystemError)
}

/*impl From<InternalDataError> for P1Error {
  fn from(value: InternalDataError) -> Self {
    P1Error::Data(DataError::Internal(value))
  }
}*/
