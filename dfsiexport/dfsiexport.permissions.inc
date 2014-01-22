<?php

// --------------------------------
// Hook_permission
// --------------------------------
// créee les permissions 
// specifiques au module dfsiexport
// --------------------------------
function dfsiexport_permission() 
{
    return array
    (
        'export dfsi data' => array
        (
        'title' => t('exporter les données de dfsi'),
        'description' => t("permission d'exportation")
        ),
    );
}