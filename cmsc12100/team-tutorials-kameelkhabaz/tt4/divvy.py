'''
Team Tutorial #4: Divvy Bike example
'''

import csv
import sys
import time
import math
import operator

class Location:
    """
    Represents a geographic location
    """

    def __init__(self, latitude, longitude):
        """
        Constructor

        Args:
        - latitude, longitude: (float) The coordinates
          for this location.
        """
        self.latitude = latitude
        self.longitude = longitude

    def distance_to(self, other):
        """
        Computes the distance to another location using the
        Haversine Formula

        Args:
        - other: (Location object) Another location

        Returns: (float) the distance to the other location
        """
        diff_latitude = math.radians(other.latitude - self.latitude)
        diff_longitude = math.radians(other.longitude - self.longitude)

        a = math.sin(diff_latitude/2) * math.sin(diff_latitude/2) + \
            math.cos(math.radians(self.latitude)) * \
            math.cos(math.radians(other.latitude)) * \
             math.sin(diff_longitude/2) * math.sin(diff_longitude/2)
        d = 2 * math.asin(math.sqrt(a))

        return 6371000.0 * d


    def __str__(self):
        """
        Produces a string representation of the location.

        Args: None

        Returns: String representation of the location
        """
        if self.latitude < 0.0:
            lat = "S"
        else:
            lat = "N"

        if self.longitude < 0.0:
            lon = "W"
        else:
            lon = "E"

        return "({:.3f} {}, {:.3f} {})".format(abs(self.latitude),
                                               lat,
                                               abs(self.longitude),
                                               lon)


class DivvyStation:
    """
    Represents a single Divvy station.

    See constructor for description of attributes.
    """

    def __init__(self, station_id, name, latitude, longitude,
                 dpcapacity, landmark, online_date):
        """
        Constructor.

        The parameters to the constructor correspond to the fields
        in the Divvy station file.

        Args:
        - id: (integer) A unique integer identifier.
        - name: (string) A descriptive name (e.g., "State St & Harrison St")
        - latitude: (float) Latitude of the station.
        - longitude: (float) Longitude of the station.
        - dpcapacity: (integer) The number of total docks at each station
          as of 2/7/2014
        - landmark: (integer) An undocumented attribute
        - online_date: (string) Date the station went live in the system
              (e.g., "6/28/2013")
        """
        self.station_id = station_id
        self.name = name
        self.location = Location(latitude, longitude)
        self.dpcapacity = dpcapacity
        self.landmark = landmark
        self.online_date = online_date


    def distance_to(self, other_station):
        '''
        Computes the distance to another station.

        Args:
        - other_station: (DivvyStation) Another station

        Returns: (float) distance "as the crow flies" from this
          station to other_station (in meters)
        '''
        d = self.location.distance_to(other_station.location)

        return d

    def __repr__(self):
        """
        Produces a string representation of the station.

        Args: None

        Returns: String representation of the station
        """
        s = "<DivvyStation {}: {}>"
        s = s.format(self.station_id, self.name)
        return s


class DivvyTrip:
    """
    Represents a single Divvy trip.

    See constructor for description of attributes.
    """

    def __init__(self, trip_id, starttime, stoptime, bikeid,
                 tripduration, from_station, to_station,
                 usertype, gender, birthyear):
        """
        Constructor

        The parameters to the constructor correspond to the fields
        in the Divvy trip file.

        Args:
        - trip_id: (integer) A unique identifier for the trip.
        - starttime, and stoptime: (string) Date and time for the start
          and end time of the trip.
        - bikeid: (integer) A unique identifier for the bike used in this trip.
        - tripduration: (integer) The duration (in seconds) of the trip.
        - from_station_id, to_station_id: (integer) The identifiers of the
          origin and destination stations.
        - from_station_name, to_station_name: The names of the origin and
          destination stations.
        - usertype: This field will be either Customer or Subscriber.
          A "customer" is a rider who purchased a 24-Hour Pass, and a
          "subscriber" is a rider who purchased an Annual Membership.
        - gender: The gender of the rider. This field only has a value
          when the rider is a subscriber.
        - birthday: The date of birth of the rider. This field only has a
          value when the rider is a subscriber.
        """

        self.trip_id = trip_id
        self.starttime = starttime
        self.stoptime = stoptime
        self.bikeid = bikeid
        self.tripduration = tripduration
        self.from_station = from_station
        self.to_station = to_station
        self.usertype = usertype
        self.gender = gender
        self.birthyear = birthyear

    def get_distance(self):
        """
        Returns the distance from the origin station to the
        destination station
        """
        return self.from_station.distance_to(self.to_station)

    def __repr__(self):
        """
        Produces a string representation of the trip.

        Args: None

        Returns: String representation of the trip
        """
        s = "<DivvyTrip {}: Station {} to Station {}>"
        s = s.format(self.trip_id,
                     self.from_station.station_id,
                     self.to_station.station_id)
        return s


class DivvyData:
    """
    Encapsulates the entire Divvy dataset.

    A DivvyData object has three attributes:
    - stations: A dictionary mapping station IDs to DivvyStation objects
    - trips: A list of DivvyTrip objects, in the same order in which they
             appear in the trips file (sorted by trip start time)
    - bikeids: A set of bike IDs in the dataset
    """

    def __init__(self, stations_filename, trips_filename):
        """
        Constructor.

        Args:
        - stations_filename: (string) Path of Divvy stations file
        - trips_filename: (string) Path of Divvy trips file
        """

        self.stations = DivvyData.read_stations_file(stations_filename)
        self.trips = self.read_trips_file(trips_filename)

        self.bikeids = set()
        for t in self.trips:
            self.bikeids.add(t.bikeid)

    # A method that is useful for the class but does not use any
    # instance attributes or instance methods
    @staticmethod
    def read_single_station(row):
        """
        Create a DivvyStation object based on a line
        from the stations CSV file

        Args:
        - row: (list of strings) Values in a single row of the
          stations CSV file

        Returns: DivvyStation object constructed with the values
          in the provided row.
        """

        if len(row) < 7:
            print("Error in parsing line: " + ",".join(row))
            return None

        try:
            station_id = int(row[0])
            name = row[1]

            latitude = float(row[2])
            longitude = float(row[3])

            dpcapacity = int(row[4])
            landmark = int(row[5])

            date = time.strptime(row[6], "%m/%d/%Y")
        except ValueError as e:
            print("Error in parsing data: " + str(e))
            return None

        return DivvyStation(station_id, name, latitude,
                            longitude, dpcapacity, landmark, date)

    # A method that is useful for the class but does not use any
    # instance attributes or instance methods
    @staticmethod
    def read_stations_file(filename):
        """
        Read a Divvy stations file.

        Args:
        - filename: (string) Path to station file

        Returns: (dictionary: integer -> DivvyStation) A dictionary that
          maps station identifiers to DivvyStation objects
        """
        stations = {}
        with open(filename) as f:
            reader = csv.reader(f)
            # read the header row.
            next(reader)
            for row in reader:
                station = DivvyData.read_single_station(row)
                if not station:
                    print("Error reading station: " + ",".join(row))
                    sys.exit(0)
                else:
                    stations[station.station_id] = station

        return stations

    def read_single_trip(self, row):
        """
        Create a DivvyTrip object based on a line
        from the trips CSV file

        Args:
        - row: (list of strings) Values in a single row of the
          trips CSV file

        Returns: DivvyTrip object constructed with the values
          in the provided row.
        """

        try:
            trip_id = int(row[0])

            starttime = time.strptime(row[1], "%Y-%m-%d %H:%M")
            endtime = time.strptime(row[2], "%Y-%m-%d %H:%M")

            bikeid = int(row[3])
            tripduration = int(row[4])

            station_id = int(row[5])
            if station_id not in self.stations:
                print("Encountered unknown station: " + str(station_id))
                return None
            from_station = self.stations[station_id]
            # Skip the station name (row[6]). We do not use it.

            station_id = int(row[7])
            if station_id not in self.stations:
                print("Encountered unknown station: " + str(station_id))
                return None
            to_station = self.stations[station_id]
            # Skip the station name (row[8]). We do not use it.

            usertype = row[9]
            gender = None
            birthyear = 0

            if usertype == "Subscriber":
                gender = row[10]
                if gender not in ["", "Male", "Female"]:
                    print("Encountered unknown gender: " + gender)
                    return None

                if len(row[11]) > 0:
                    birthyear = int(row[11])
        except ValueError as e:
            print("Error in parsing line: " + str(e))
            return None

        return DivvyTrip(trip_id, starttime, endtime, bikeid,
                         tripduration, from_station, to_station,
                         usertype, gender, birthyear)

    def read_trips_file(self, filename):
        """
        Read a Divvy trips file.

        Args:
        - filename: (string) Path to trips file

        Returns: (list of DivvyTrip) A list with all the trips in the file.
        """
        trips = []
        with open(filename) as f:
            reader = csv.reader(f)
            # read the header row.
            next(reader)
            for row in reader:
                trip = self.read_single_trip(row)
                if not trip:
                    print("Error reading trip: " + ",".join(row))
                    sys.exit(0)
                else:
                    trips.append(trip)

        return trips

    def get_number_stations(self):
        """Returns the number of stations in the dataset"""
        return len(self.stations)

    def get_number_trips(self):
        """Returns the number of trips in the dataset"""
        return len(self.trips)

    def get_total_distance(self):
        """Returns the total distance of all the Divvy trips"""
        total_distance = 0.0
        for trip in self.trips:
            total_distance += trip.get_distance()

        return total_distance

    def get_total_duration(self):
        """Computes the total duration, in seconds, of all the Divvy trips"""
        total_duration = 0.0

        for trip in self.trips:
            total_duration += trip.tripduration

        return total_duration

    def get_bike_times(self):
        """
        Computes, for every bike in the Divvy dataset, the sum of the
        duration of all the trips taken by that bike.

        Returns a dictionary mapping bike identifiers (integer) to
        a duration in seconds (integer)
        """

        bike_times = {}

        for trip in self.trips:
            bike_times[trip.bikeid] = bike_times.get(trip.bikeid,0) + \
                                      trip.tripduration
                                  
        return bike_times


    def get_bike_movements(self):
        """
        Returns a dictionary mapping bike identifiers (integer)
        to a list of tuples, where each tuple represents that bike
        being moved from one station to another.

        Each tuple contains three values: the station the bike was
        moved from (DivvyStation object), the station the bike was
        moved to (DivvyStation object), and the difference in capacity
        between the two stations (more specifically, the capacity
        of the station the bike was moved to minus the capacity
        of the station the bike was moved from). Note that this
        will be an integer that can be either positive or negative.

        Note that the dictionary must also include entries for
        the bikes that have not been moved at all (those entries
        will just map to an empty list)
        """
        rv = {}
        last_trips = {} # map bike id to last trip the bike took
        for bikeid in self.bikeids:
            rv[bikeid] = []
        
        for trip in self.trips:
            
            if trip.bikeid in last_trips and last_trips[trip.bikeid].to_station != trip.from_station:
                last_trip = last_trips[trip.bikeid]
                dpcapacity = trip.from_station.dpcapacity - \
                                 last_trip.to_station.dpcapacity

                rv[trip.bikeid].append((last_trip.to_station,trip.from_station,dpcapacity))
            last_trips[trip.bikeid] = trip

        return rv


MINUTE = 60
HOUR = 60 * MINUTE
DAY = 24 * HOUR

def time_str(t):
    """
    Converts a time in seconds to a string representation
    in days, hours, minutes, seconds.

    Args:
    - t: (integer) A time in seconds

    Returns: (string) A string representation.
    """

    t = int(t)
    days = t // DAY
    hours = (t % DAY) // HOUR
    minutes = (t % HOUR) // MINUTE
    seconds = t % MINUTE

    if days == 0:
        return "{}h {}m {}s".format(hours, minutes, seconds)

    return "{}d {}h {}m {}s".format(days, hours, minutes, seconds)


def go(station_filename, trip_filename):
    """
    Print some statistics about the Divvy files.
    """
    data = DivvyData(station_filename, trip_filename)

    # Number of stations and trips
    print("# of stations:", data.get_number_stations())
    print("# of trips:", data.get_number_trips())

    print()

    # Average duration of trip
    print("The aggregate total duration of all Divvy trips in 2013 was",
          time_str(data.get_total_duration()))

    print("The average duration of a Divvy trip in 2013 was",
          time_str(data.get_total_duration() / data.get_number_trips()))

    print()

    # Total and average distance
    s = ("The total distance travelled by all the Divvy bikes in 2013"
         " was {:,.2f} kilometers.")
    print(s.format(data.get_total_distance()/1000.0))

    s = ("The average distance travelled in a single trip"
         " in 2013 was {:,.2f} meters.")
    print(s.format(data.get_total_distance()/data.get_number_trips()))

    print()

    # Bike usage times
    bikes = data.get_bike_times()

    avg_total = sum(bikes.values()) / len(bikes)
    print("The average total usage of a bike is", time_str(avg_total))

    sorted_times = sorted(bikes.items(), key=operator.itemgetter(1))
    bikeid, max_total = sorted_times[-1]
    s = "The most used bike is {}, used a total of {}"
    print(s.format(bikeid, time_str(max_total)))

    print()

    # Bike movements
    bm = data.get_bike_movements()

    moves_per_bike = [len(v) for k, v in bm.items()]
    avg_moves = sum(moves_per_bike) / len(moves_per_bike)
    s = "The average number of times a bike was moved was {:.2f}"
    print(s.format(avg_moves))

    cap_diffs = []
    for movs in bm.values():
        for _, _, cap_diff in movs:
            cap_diffs.append(cap_diff)
    avg_cap_diff = sum(cap_diffs) / len(cap_diffs)
    tmp = sum([(c-avg_cap_diff)**2 for c in cap_diffs])
    stdev_cap_diff = math.sqrt(tmp / len(cap_diffs))

    if avg_cap_diff > 0:
        cap_str = "{:.2f} more".format(avg_cap_diff)
    elif avg_cap_diff < 0:
        cap_str = "{:.2f} fewer".format(avg_cap_diff)
    elif avg_cap_diff == 0:
        cap_str = "the same number of"

    print("On average, a bike is moved to a station with", cap_str, "docks")
    print("(Standard deviation: {:.2f})".format(stdev_cap_diff))


if __name__ == "__main__":
    if len(sys.argv) == 3:
        station_filename_arg = sys.argv[1]
        trip_filename_arg = sys.argv[2]
    else:
        print("usage: python {} <stationFile> <tripFile>".format(sys.argv[0]))
        sys.exit(0)

    go(station_filename_arg, trip_filename_arg)
