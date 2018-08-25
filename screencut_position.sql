/*
Navicat MySQL Data Transfer

Source Server         : 192.168.1.125_3406
Source Server Version : 50527
Source Host           : 192.168.1.125:3406
Source Database       : vms

Target Server Type    : MYSQL
Target Server Version : 50527
File Encoding         : 65001

Date: 2017-08-16 15:42:52
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for screencut_position
-- ----------------------------
DROP TABLE IF EXISTS `screencut_position`;
CREATE TABLE `screencut_position` (
  `cameraID` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,
  `left` int(4) DEFAULT NULL,
  `top` int(4) DEFAULT NULL,
  `right` int(4) DEFAULT NULL,
  `bottom` int(4) DEFAULT NULL,
  `spreadHeiMin` float(20,5) DEFAULT NULL,
  `spreadHeiMax` float(20,5) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of screencut_position
-- ----------------------------
INSERT INTO `screencut_position` VALUES ('3301061000266', '0', '3', '0', '47', '0.00000', '9.00000');
INSERT INTO `screencut_position` VALUES ('3301061000267', '0', '50', '0', '0', '0.00000', '9.00000');
INSERT INTO `screencut_position` VALUES ('3301061000266', '0', '11', '0', '39', '9.00000', '18.00000');
INSERT INTO `screencut_position` VALUES ('3301061000267', '0', '40', '0', '10', '9.00000', '18.00000');
INSERT INTO `screencut_position` VALUES ('3301061000266', '0', '20', '0', '30', '18.00000', '24.00000');
INSERT INTO `screencut_position` VALUES ('3301061000267', '0', '32', '0', '18', '18.00000', '24.00000');
INSERT INTO `screencut_position` VALUES ('3301061000266', '0', '28', '0', '22', '24.00000', '25.00000');
INSERT INTO `screencut_position` VALUES ('3301061000267', '0', '25', '0', '25', '24.00000', '25.00000');
INSERT INTO `screencut_position` VALUES ('3301061000266', '0', '19', '0', '31', '25.00000', '30.00000');
INSERT INTO `screencut_position` VALUES ('3301061000267', '0', '34', '0', '16', '25.00000', '30.00000');
INSERT INTO `screencut_position` VALUES ('3301061000266', '0', '27', '0', '23', '30.00000', '37.00000');
INSERT INTO `screencut_position` VALUES ('3301061000267', '0', '27', '0', '23', '30.00000', '37.00000');
INSERT INTO `screencut_position` VALUES ('3301061000266', '0', '32', '0', '18', '37.00000', '40.00000');
INSERT INTO `screencut_position` VALUES ('3301061000267', '0', '21', '0', '29', '37.00000', '40.00000');
INSERT INTO `screencut_position` VALUES ('3301061000266', '0', '42', '0', '8', '40.00000', '42.00000');
INSERT INTO `screencut_position` VALUES ('3301061000267', '0', '13', '0', '37', '40.00000', '42.00000');
INSERT INTO `screencut_position` VALUES ('3301061000266', '0', '50', '0', '0', '42.00000', '60.00000');
INSERT INTO `screencut_position` VALUES ('3301061000267', '0', '0', '0', '50', '42.00000', '60.00000');
