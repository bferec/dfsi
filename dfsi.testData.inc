<?php

// ------------------------------------------------
// @(#) dfsi.testdata.inc
// @(#) (c) bruno Ferec
// @(#) 17/01/2014
// ------------------------------------------------

if( !defined("_DFSI_MODULE_NAME") ) { define( "_DFSI_MODULE_NAME" , "dfsi" ); }                                               // nom du module
if( !defined("_DFSI_INCLUDE_EXTENSION") ) { define( "_DFSI_INCLUDE_EXTENSION" , "inc" ); }                                    // extension pour les fichiers include php


module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.define");
module_load_include( _DFSI_INCLUDE_EXTENSION , _DFSI_MODULE_NAME , "dfsi.api");


// ------------------------------------------
// suppression des donnees de test
// ------------------------------------------
function dfsi_Delete_Test_Data()
{
   variable_set('dfsi_sem_Update', true);
   
   $testContainersNids = variable_get( 'dfsi_Container_TestList' , array() );
   $testServicesNids = variable_get( 'dfsi_Service_TestList' , array() );
   
   node_delete_multiple($testContainersNids);
   node_delete_multiple($testServicesNids);   
   
   variable_set('dfsi_sem_Update', false);   
}

 // ------------------------------------------
// creation des donnees de test
// ------------------------------------------
function dfsi_create_Test_Data()
{         
    variable_set('dfsi_sem_Update', true);  // pas de controle d'integrité sur les données de test
   
    $containerDefList = array
    (
        'TEST_DC01'        => array( 'title' => "TEST_DC01"         , 'body' => "Contrôleur de domaine"     , 'field_statut' => 'Ok'  ,  'field_criticite' => 'Important'  ),
        'TEST_DC02'        => array( 'title' => "TEST_DC02"         , 'body' => "Contrôleur de domaine"     , 'field_statut' => 'Ok'  ,  'field_criticite' => 'Important'  ),
        "TEST_Datacenter"  => array( 'title' => "TEST_Datacenter"   , 'body' => "Datacenter de l'entreprise" , 'field_statut' => 'Ok'  ,  'field_criticite' => 'Indispensable' ),       
    );
    
    $serviceDefList = array
    (
        'TEST_Domaine_DFSI.CORP' => array( 'title' => "TEST_Domaine_DFSI.CORP" , 'body' => 'Domaine interne groupe'            , 'field_statut' => 'Ok' , 'field_criticite' => 'Important' ),
        'Fsmo DFSI.CORP'         => array( 'title' => "Fsmo"                   , 'body' => "serveur FSMO du domaine DFSI.CORP" , 'field_statut' => 'Ok' , 'field_criticite' => 'Important' ),
        'TEST_BACKUP_SOLUTION'   => array( 'title' => "TEST_BACKUP_SOLUTION"   , 'body' => "sauvegarde messagerie et archives" , 'field_statut' => 'Ok' , 'field_criticite' => 'Important' ),       
    );
    
    $testContainersNids = array();
    $testServicesNids = array();

    // -----------------------------------
    // creation des containers de test
    // -----------------------------------      
    foreach( $containerDefList as $newContainer )
    {
        
        $node = dfsi_AddContainer( $newContainer['title'] , $newContainer['body'] , $newContainer['field_statut'] , $newContainer['field_criticite']  , $language = LANGUAGE_NONE);
        $testContainersNids[] = dfsi_GetNidOf( $node );
    }
    // -----------------------------------
    // creation des services de test
    // -----------------------------------      
    foreach( $serviceDefList as $newService )
    {
        $node = dfsi_AddService( $newService['title'] , $newService['body'] , $newService['field_statut'] , $newService['field_criticite']   , $language = LANGUAGE_NONE);
        $testServicesNids[] = dfsi_GetNidOf( $node );
    } 
    // -----------------------------
    // mise en place des taxonomies
    // -----------------------------
    dfsi_Set_Taxonomie_Data( $testContainersNids , $testServicesNids ); 
    // -----------------------------
    // mise en place des dependances
    // -----------------------------
    dfsi_create_dependance_Data( $testContainersNids , $testServicesNids );
   
    variable_set( 'dfsi_Container_TestList' , $testContainersNids );
    variable_set( 'dfsi_Service_TestList' , $testServicesNids );
   
    variable_set('dfsi_sem_Update', false);
}


// ------------------------------------------
// attach terms
// ------------------------------------------
function dfsi_Set_Taxonomie_Data( $testContainersNids , $testServicesNids )
{
    // ------------------
    // type de services
    // ------------------
    $testDomaine = node_load($testServicesNids[0]);   
    dfsi_SetTaxonomyServiceType( $testDomaine , 'Contrôleur de domaine Active directory' );
    
    $fsmo = node_load($testServicesNids[1]);  
    dfsi_SetTaxonomyServiceType( $fsmo , "Maitre d'opération active directory" );
    
    $backServ = node_load($testServicesNids[2]);  
    dfsi_SetTaxonomyServiceType( $backServ , 'Service Backup' );    
    // ------------------
    // type de containers
    // ------------------
    $dc01 = node_load($testContainersNids[0]);  
    dfsi_SetTaxonomyServiceType( $dc01 , 'Serveur' );

    $dc02 = node_load($testContainersNids[1]);  
    dfsi_SetTaxonomyServiceType( $dc02 , 'Serveur' );    
    
    $datacenter = node_load($testContainersNids[2]);  
    dfsi_SetTaxonomyServiceType( $datacenter , 'Salle informatique' );    
}

// ------------------------------------------
// Mise en place des  dependances et contenus
// ------------------------------------------
function dfsi_create_dependance_Data( $testContainersNids , $testServicesNids )
{
    // chargement des noeuds concernées
    $dataCenter = node_load($testContainersNids[2]);    
    $dc01 =  node_load($testContainersNids[0]);
    $dc02 =  node_load($testContainersNids[1]);
    $fsmo = node_load($testServicesNids[1]); 
    $backServ = node_load($testServicesNids[2]); 
    //
    // TEST_DC02 et TEST_DC01 sont contenus dans TEST_DATACENTER et en dependent
    dfsi_AddContent( $testContainersNids[0],  $dataCenter );
    dfsi_AddContent( $testContainersNids[1],  $dataCenter );    
    dfsi_AddDependancies($testContainersNids[0],  $dataCenter );
    dfsi_AddDependancies($testContainersNids[1],  $dataCenter );   
    //
    // durant la generation des données de test
    // les contraintes d'intégrité sont desactivées
    // il faut donc ajouter datacenter aux ancetres
    dfsi_AddAncestors(dfsi_GetNidOf($dataCenter),  $dc01 );
    dfsi_AddAncestors(dfsi_GetNidOf($dataCenter),  $dc02 );    
    //
    // le domaine ENI depends de TEST_DC01 OU CTEST_DC02 : groupe de dependances
    $testDomaine = node_load($testServicesNids[0]);      
    $testDomaine -> field_groupe_de_dependances[$testDomaine->language][]['nid'] = $testContainersNids[0];
    $testDomaine -> field_groupe_de_dependances[$testDomaine->language][]['nid'] = $testContainersNids[1];
    //
    // le service FSMO depends directement de TEST_DC01
    dfsi_AddAncestors(dfsi_GetNidOf($dc01),  $fsmo );
    dfsi_AddDependancies(dfsi_GetNidOf($fsmo),  $dc01 );
    //
    // le service Backup  dépends  directement de TEST_DC02
    dfsi_AddAncestors(dfsi_GetNidOf($dc02),  $backServ );
    dfsi_AddDependancies(dfsi_GetNidOf($backServ),  $dc02 );  
    //
    // sauvegarde des noeuds modifiés
    node_save($dataCenter);    
    node_save($dc01);
    node_save($dc02);
    node_save($fsmo);
    node_save($backServ);
    node_save($testDomaine);    
}

