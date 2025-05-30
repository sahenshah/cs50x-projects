-- Keep a log of any SQL queries you execute as you solve the mystery.

SELECT * FROM crime_scene_reports WHERE day = '28' AND month = '7' AND year = '2024' AND street = 'Humphrey Street';

SELECT * FROM interviews WHERE (name = 'Ruth' OR name = 'Eugene' OR name = 'Raymond') AND day = '28' AND month = '7' AND year = '2024';

SELECT
    bsl.year,
    bsl.month,
    bsl.hour,
    bsl.minute,
    bsl.activity,
    p.name AS vehicle_owner
FROM bakery_security_logs bsl
JOIN people p ON bsl.license_plate = p.license_plate
WHERE day = '28' AND month = '7' AND year = '2024' AND hour = '10' and minute < '25' and activity = 'exit';

SELECT
    atm.id,
    atm.year,
    atm.month,
    atm.day,
    atm.atm_location,
    atm.transaction_type,
    atm.amount,
    p.name AS account_holder
FROM atm_transactions atm
JOIN bank_accounts ba ON atm.account_number = ba.account_number
JOIN people p ON ba.person_id = p.id
WHERE day = '28' AND month = '7' AND year = '2024' AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';

SELECT
    pc.id,
    pc.year,
    pc.month,
    pc.day,
    pc.duration,
    caller.name AS caller_name,
    receiver.name AS receiver_name
FROM phone_calls pc
JOIN people caller ON pc.caller = caller.phone_number
JOIN people receiver ON pc.receiver = receiver.phone_number
WHERE day = '28' AND month = '7' AND year = '2024' AND duration < '60';

SELECT  f.id,
        f.year,
        f.month,
        f.day,
        f.hour,
        f.minute,
        origin.city AS origin_city,
        destination.city AS destination_city
   FROM flights f
   JOIN airports origin ON f.origin_airport_id = origin.id
   JOIN airports destination ON f.destination_airport_id = destination.id
   WHERE day = '29' AND month = '7' AND year = '2024'
   ORDER BY hour;

SELECT
    pa.flight_id,
    pa.seat,
    p.name AS passenger_name
FROM passengers pa
JOIN people p ON pa.passport_number = p.passport_number
WHERE flight_id = 36;

