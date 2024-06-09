CREATE DATABASE IF NOT EXISTS sm_app;
USE sm_app;

CREATE TABLE `events` (
  `id`           INT                NOT NULL AUTO_INCREMENT,
	`timestamp`    TIMESTAMP          DEFAULT NOW(),
  `building_id`  INT                NOT NULL,
	`epoch`        INT                NOT NULL,
  `point_id`     INT                NOT NULL,
	`axis`         ENUM('x','y','z')  NOT NULL,
	`frequency`    DECIMAL(6,3)       NOT NULL,
	`value`        DECIMAL(6,3)       NOT NULL,

  PRIMARY KEY (`id`)
);
