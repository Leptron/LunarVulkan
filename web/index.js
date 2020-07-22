const fastify = require('fastify')({ logger: true });

const mongoWrapper = require("./mongo-handler");
const LunarAuth = require("./user-auth.js");

function exitHandler() {
    mongoWrapper.cleanup();
    process.exit(0);
}

fastify.get('/', async (req, reply) => {
    return { LunarServiceWorking: true };
})


//user auth
fastify.post('/signup', async(req, reply) => {
    const email = req.body.email;
    const psswd = req.body.psswd;

    LunarAuth.login_auth(email, psswd);
    
    return { LunarSignupService: "unhandled" };
})

const start = async () => {
    mongoWrapper.initResources();
    try {
        await fastify.listen(3000);
        fastify.log.info(`server listening on ${fastify.server.address().port}`)
    } catch(err) {
        fastify.log.error(err)
        process.exit(1);
    }
}

start();

process.on('exit', exitHandler.bind());
process.on('SIGINT', exitHandler.bind());