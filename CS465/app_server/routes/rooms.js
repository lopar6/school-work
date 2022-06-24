const express = require('express');
const router = express.Router();
const controller= require('../controllers/rooms');

/* GET rooms page. */
router.get('/', controller.rooms);

module.exports = router;