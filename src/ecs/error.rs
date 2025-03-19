#[derive(Debug)]
pub enum ComponentError {
  MismatchedComponentType,
  MissingContainer,
  NotFoundForEntity
}

#[derive(Debug)]
pub enum EntityError {
  NotFound
}

#[derive(Debug)]
pub enum P1Error {
  Component(ComponentError),
  Entity(EntityError),
  // SHOULD NEVER GET THAT ERROR
  // Happens when a component wasn't found for an entity in the component manager
  // but the entity manager thinks it exists for that entity
  MismatchedComponents,
  // Happens when a component type is already registered with a specific entity
  // and an attempt was made to add another of the same type
  // In this engine, we only want one component per type of component per entity
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
