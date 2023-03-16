# syntax=docker/dockerfile:1

FROM node:19.7.0-alpine3.17 AS base

WORKDIR /app

COPY [ "package.json", "yarn.lock*", "./" ]

FROM base AS dev
ENV NODE_ENV=dev
RUN yarn install
COPY . .
CMD [ "yarn", "start:dev" ]

FROM base AS prod
ENV NODE_ENV=production
RUN yarn install --production
COPY . .
RUN yarn global add @nestjs/cli
RUN yarn build
CMD [ "yarn", "start:prod" ]