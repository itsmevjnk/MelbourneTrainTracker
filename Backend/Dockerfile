FROM node:22-alpine

RUN apk add --no-cache curl

RUN mkdir -p /home/node/app/node_modules && chown -R node:node /home/node/app

WORKDIR /home/node/app
COPY --chown=node:node package*.json ./

USER node
RUN npm install

COPY --chown=node:node . .

# health check port
# EXPOSE 3000

# connection environment variables (only CONNECT_STR is required for initdb script)
#  - API_KEY: GTFS Realtime API key
#  - CONNECT_STR: PostgreSQL database connection URL
#  - MQTT_BROKER: MQTT broker URL (including protocol schema, e.g. mqtt://)
#  - MQTT_USERNAME: MQTT authentication username
#  - MQTT_PASSWORD: MQTT authentication password
#  - MQTT_TOPIC: the MQTT topic to publish information to

# health check
HEALTHCHECK --interval=30s --timeout=3s --start-period=5s --retries=3 CMD curl -f http://127.0.0.1:3000/ || exit 1

# the script can be set through environment variable to initdb for the GTFS schedule update script
ENV SCRIPT=start
CMD [ "sh", "-c", "npm run $SCRIPT" ]
