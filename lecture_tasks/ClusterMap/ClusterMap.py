import json
import shapely
import matplotlib.pyplot as plt

INPUT_DATA_FILE = 'json/base.json'

# SOURCE: https://gist.github.com/hrbrmstr/91ea5cc9474286c72838
GEO_JSON_FILE = 'json/mapdata.json'



def _get_pure_north_america(polygon):
    """Finds the largest polygon via vector multiplication. The source data contains not only North America but also Greenland and some ither stuff"""
    max_area = 0
    max_poly = []
    for i in range(len(polygon)):
        points = polygon[i][0]
        area = shapely.Polygon(points).area
        if area > max_area:
            max_area = area
            max_poly = points
    return max_poly


def get_north_america():
    """Returns Polygon of for  North America"""
    global GEO_JSON_FILE
    with open(GEO_JSON_FILE, "r") as file:
        continents = json.load(file)
        north_america_polygon = continents["features"][1]["geometry"]["coordinates"]
    return shapely.Polygon(_get_pure_north_america(north_america_polygon))


def get_origins():
    """Parses origins from JSON file """
    global INPUT_DATA_FILE
    with open(INPUT_DATA_FILE, "r") as f:
        sites = json.load(f)
        lons = []
        lats = []
        for site in sites:
            lons.append(site["location"]["lon"])
            lats.append(site["location"]["lat"])
        return lons, lats, sites


def process_file():
    """Parses points from the file, draws'em all and colors points in North America"""
    polygon = get_north_america()
    north_america_sites = []
    res = 0
    lons, lats, sites = get_origins()
    for site in sites:
        p = shapely.Point((site["location"]["lon"], site["location"]["lat"]))
        if polygon.contains(p):
            res += 1
            north_america_sites.append(site)
    flons = []
    flats = []
    for site in north_america_sites:
        flons.append(site["location"]["lon"])
        flats.append(site["location"]["lat"])
    plt.scatter(lons, lats)  # All the points
    plt.scatter(flons, flats)  # Points in North America
    plt.show()


def main():
    """The main function"""
    process_file()


if __name__ == '__main__':
    """The main function invocation"""
    main()
