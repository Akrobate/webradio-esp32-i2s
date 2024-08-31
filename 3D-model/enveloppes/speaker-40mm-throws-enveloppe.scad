include <../configurations/global.scad>

/**
 * speaker28mmThrowsEnveloppe
 * @name speaker28mmThrowsEnveloppe
 * @description speaker28mmThrowsEnveloppe
 * @type enveloppe
 * @parent root
 */
module speaker40mmThrowsEnveloppe(
    speaker_facade_throw_diameter = speaker_facade_throw_diameter,
    case_external_panes_thickness = case_external_panes_thickness

) {

    translate([0, 0, -case_external_panes_thickness * 2])
    cylinder(
        d = speaker_facade_throw_diameter,
        h = case_external_panes_thickness * 4,
        $fn = 100
    );
}

speaker40mmThrowsEnveloppe();