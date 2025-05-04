SELECT 'CREATE DATABASE melbtrains'
WHERE NOT EXISTS (SELECT FROM pg_database WHERE datname='melbtrains')\gexec

\c melbtrains

CREATE SCHEMA daily;

ALTER SCHEMA daily OWNER TO postgres;

CREATE SCHEMA gtfs;

ALTER SCHEMA gtfs OWNER TO postgres;

SET default_tablespace = '';

SET default_table_access_method = heap;

CREATE TABLE daily.timetable (
    trip_id character varying(32) NOT NULL,
    line character(3) NOT NULL,
    seq integer NOT NULL,
    arrival timestamp with time zone NOT NULL,
    departure timestamp with time zone NOT NULL,
    last_updated timestamp with time zone DEFAULT CURRENT_TIMESTAMP NOT NULL,
    station character(3) NOT NULL,
    next_station character(3),
    prev_station character(3),
    CONSTRAINT timetable_time CHECK ((arrival <= departure))
);

ALTER TABLE daily.timetable OWNER TO postgres;

CREATE TABLE gtfs.calendar (
    start_date date NOT NULL,
    end_date date NOT NULL,
    monday boolean NOT NULL,
    tuesday boolean NOT NULL,
    wednesday boolean NOT NULL,
    thursday boolean NOT NULL,
    friday boolean NOT NULL,
    saturday boolean NOT NULL,
    sunday boolean NOT NULL,
    id character varying(16) NOT NULL,
    CONSTRAINT calendar_date CHECK ((start_date <= end_date))
);

ALTER TABLE gtfs.calendar OWNER TO postgres;

CREATE TABLE gtfs.stops (
    id integer NOT NULL,
    station character(3) NOT NULL
);

ALTER TABLE gtfs.stops OWNER TO postgres;

CREATE TABLE gtfs.timetable (
    trip_id character varying(32) NOT NULL,
    stop_id integer NOT NULL,
    arrival interval NOT NULL,
    departure interval NOT NULL,
    seq integer NOT NULL,
    CONSTRAINT timetable_time CHECK ((arrival <= departure))
);

ALTER TABLE gtfs.timetable OWNER TO postgres;

CREATE TABLE gtfs.trips (
    id character varying(32) NOT NULL,
    calendar character varying(16) NOT NULL
);


ALTER TABLE gtfs.trips OWNER TO postgres;

ALTER TABLE ONLY daily.timetable
    ADD CONSTRAINT timetable_pkey PRIMARY KEY (trip_id, seq);

ALTER TABLE ONLY gtfs.calendar
    ADD CONSTRAINT id PRIMARY KEY (id);

ALTER TABLE ONLY gtfs.stops
    ADD CONSTRAINT stops_pkey PRIMARY KEY (id);

ALTER TABLE ONLY gtfs.timetable
    ADD CONSTRAINT timetable_pkey PRIMARY KEY (seq, trip_id);

ALTER TABLE ONLY gtfs.trips
    ADD CONSTRAINT trips_pkey PRIMARY KEY (id);

CREATE INDEX fki_timetable_stop ON gtfs.timetable USING btree (stop_id);

CREATE INDEX fki_trips_calendar ON gtfs.trips USING btree (calendar);

ALTER TABLE ONLY gtfs.timetable
    ADD CONSTRAINT timetable_stop FOREIGN KEY (stop_id) REFERENCES gtfs.stops(id);

ALTER TABLE ONLY gtfs.timetable
    ADD CONSTRAINT timetable_trip FOREIGN KEY (trip_id) REFERENCES gtfs.trips(id);

ALTER TABLE ONLY gtfs.trips
    ADD CONSTRAINT trips_calendar FOREIGN KEY (calendar) REFERENCES gtfs.calendar(id);
