
/**
 * getPointSize
 * @returns Point size (mil)
 */
function getPointSize() = 2.54;

/**
 * Convert
 * @returns dimension in milimeters from points numbers
 */
function convertPointstoMilimeters(points) = points * getPointSize();

/**
 * getSizeFromPointCount
 * @returns dimension in milimeters from points numbers
 */
function getSizeFromPointCount(point_count) = convertPointstoMilimeters(point_count + 1);



