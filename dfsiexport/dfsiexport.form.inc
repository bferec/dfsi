<?php

// ------------------------------------------------
// @(#) dfsiexport.form.inc
// @(#) modification des formulaires
// @(#) (c) Amael Hervoche
// @(#) 24/01/2014
// ------------------------------------------------

function dfsiexport_form_alter(&$form, &$form_state, $form_id) 
{
    if($form['#form_id']  == 'dfsiexport_ExportForm' ) //
    {
        $choixExport = variable_get('dfsiexport_choixExport', array());
        foreach($choixExport as $i )
        {
            $form['dfsiexport_choixExport'][$i]['#default_value'] = true;
        }
    }
}
