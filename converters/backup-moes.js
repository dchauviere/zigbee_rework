const fz = require("zigbee-herdsman-converters/converters/fromZigbee");
const tz = require("zigbee-herdsman-converters/converters/toZigbee");
const tuya = require('zigbee-herdsman-converters/lib/tuya')
const legacy = require('zigbee-herdsman-converters/lib/legacy')
const exposes = require('zigbee-herdsman-converters/lib/exposes');
const reporting = require('zigbee-herdsman-converters/lib/reporting');
const extend = require('zigbee-herdsman-converters/lib/modernExtend')
const e = exposes.presets;
const ea = exposes.access;


const definition = {
    fingerprint: tuya.fingerprint('TS0013', ['_TZ3000_qewo8dlz']),
    model: 'ZS-EUB',
    vendor: 'Moes',
    description: 'Smart Switch 3-Gang',
    extend: [extend.deviceEndpoints({"endpoints":{"left":1,"center":2,"right":3}}), extend.onOff({"powerOnBehavior":false,"endpointNames":["left","center","right"]})],
    meta: {multiEndpoint: true},
};


module.exports = definition;
