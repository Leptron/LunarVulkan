const fastify = require('fastify')()

fastify.get('/', async (req, reply) => {
    return { LunarServiceWorking: true };
})


//user auth
fastify.post('/login', async(req, reply) => {
    console.log("request");
    const email = req.body.email;
    const psswd = req.body.psswd;

    const db = fastify.mongo.db;
    db.collection('LunarUsers', onCollection);

    function onCollection(err, col) {
        if(err) return reply.send(err);

        col.findOne({email: email}, (err, user) => {
            if(user == null)
                reply.send({user_exists: false})
        })
    }
})



const start = async () => {
    try {
        fastify.register(require('fastify-mongodb'), {
            // force to close the mongodb connection when app stopped
            // the default value is false
            forceClose: true,
            
            url: 'mongodb+srv://lunar_server:LeptronApp$45@lunardevcluster.s70jv.azure.mongodb.net/LunarServer'
        })
        fastify.register(require('fastify-cors'), { 
            // put your options here
        })

        await fastify.listen(3000);
        fastify.log.info(`server listening on ${fastify.server.address().port}`)
    } catch(err) {
        fastify.log.error(err)
        process.exit(1);
    }
}

start();