update guanggun,var set guanggun.score = guanggun.score - (var.data - 180) * 10 where var.id = 639 and var.data > 180 and var.playerId = guanggun.playerId and guanggun.score > (var.data - 180) * 10;
