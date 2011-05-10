ALTER IGNORE TABLE `item`  ADD UNIQUE INDEX `id_ownerId_bindType` (`id`, `ownerId`, `bindType`);
