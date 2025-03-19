use thiserror::Error;

#[derive(Error, Debug)]
pub enum ComponentError {
  #[error("A component was pushed to an incompatible container.\nThis is an internal error and should never happen.")]
  MismatchedComponentType,
  #[error("No container was found to push the provided component type to.")]
  MissingContainer,
  #[error("No component of the provided type was found for the provided entity")]
  NotFoundForEntity
}

#[derive(Error, Debug)]
pub enum EntityError {
  #[error("No entities with the provided id was found.")]
  NotFound
}

#[derive(Error, Debug)]
pub enum P1Error {
  #[error(transparent)]
  Component(ComponentError),
  #[error(transparent)]
  Entity(EntityError),
  // Need typename info soon
  #[error("Cannot attach component to entity because a component of that type is already attached")]
  ComponentExistsForEntity
}

impl From<ComponentError> for P1Error {
  fn from(value: ComponentError) -> Self {
    P1Error::Component(value)
  }
}
impl From<EntityError> for P1Error {
  fn from(value: EntityError) -> Self {
    P1Error::Entity(value)
  }
}
