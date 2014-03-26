delete pet_sevensoul.* from pet_sevensoul,fighter where fighter.id < 10 and fighter.level < 80 and fighter.playerId = pet_sevensoul.playerId;
