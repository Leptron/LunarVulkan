const mongo = require('mongodb');

const mongo_url = "mongodb+srv://lunar_server:LeptronApp$45@lunardevcluster.s70jv.azure.mongodb.net/LunarServer?retryWrites=true&w=majority";
var dbObject;

function initResources() {
    mongo.MongoClient.connect(mongo_url, function(err, db) {
        if(err) throw err;

        console.log("Lunar Database : DB Connected");
        dbObject = db;
    });
}

function cleanup() {
    console.log("Lunar Database : Database Closing");
    dbObject.close();
}

module.exports.initResources = initResources;
module.exports.cleanup = cleanup;