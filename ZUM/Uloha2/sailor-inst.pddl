(define (problem islands-3)
    (:domain sailor)
    (:requirements :strips)
    (:objects 
        island sea lighthouse port town academy river woods pub - location
        sailor captain admiral - rank
        sober mood drunk addicted - alcohol_level
        shady good smugglers - acquitance
        strong defeat_pirates married drug_addict criminal_record girlfriend_fascinated - atribute
        frigate karavela boat - item
        wood golden_grain golden_coin bear_skin golden_brick map pearl cocaine flowers coconut alcohol ring - item
        )
    (:init
        (road woods river)
        (road river woods)
        (road river port)
        (road port river)
        (road port town)
        (road town port)
        
        ;*********Comment this to satisfy REQUIREMENT 1 - NO ROAD TOWN - ACADEMY********
        (road town academy)
        *********REQUIREMENT 1*************
        
        (road academy town)
        (road pub port)
        (road port pub)
        (waterway port sea)
        (waterway sea port)
        (waterway port lighthouse)
        (waterway lighthouse port)
        (waterway lighthouse sea)
        (waterway sea lighthouse)
        (waterway sea island)
        (waterway island sea)


        
        ;General crafting skills------------------------
        (craft_3 boat wood golden_grain frigate)
        (craft_3 boat wood golden_coin karavela)
        (craft golden_brick pearl ring)
        ;-----------------------------------------------
        
        ;At woods --------------------------------------
        (acquire_at woods wood)
        (acquire_at woods flowers)
        
        ;*********Comment this to satisfy REQUIREMENT 2 - NO MAP TO TRADE********
        (trade_at_shady woods alcohol map)
        ;                        *********REQUIREMENT 2***************
        
        ;-----------------------------------------------
    
        ;At river --------------------------------------
        (acquire_at river golden_grain)
        
        ;*********Comment this to satisfy REQUIREMENT 4 - NO BOAT TO STEAL********
        (steal_at river boat)
        ;                        *********REQUIREMENT 4**************
        
        ;-----------------------------------------------
        
        ;At port----------------------------------------
        (acquire_at port golden_grain)
        (craft_at port coconut golden_coin)
        (craft_at port bear_skin golden_coin)
        ;-----------------------------------------------
        
        ;At pub ----------------------------------------
        (craft_at pub golden_grain alcohol)
        ;-----------------------------------------------

        ;At town----------------------------------------
        (invest_at town golden_grain golden_coin) 
        (invest_at town golden_coin golden_brick) 
        (steal_at town golden_coin)
        ;-----------------------------------------------
        
        ;At academy-------------------------------------
        ;Nothing general happens here
        ;-----------------------------------------------

        ;At sea-----------------------------------------
        ;*********Comment this to satisfy REQUIREMENT 3 - NO PEARL IN SEA********
        (acquire_at sea pearl)
        ;                        *********REQUIREMENT 3**************
        ;-----------------------------------------------
        
        ;At lighthouse----------------------------------
        ;Nothing general happens here
        ;-----------------------------------------------
        
        
        ;At island--------------------------------------
        (acquire_at island coconut)
        (acquire_at island wood)
        (craft_at island map cocaine)
        ;-----------------------------------------------
        
        (at port)
	(alcohol_level sober)
    )
    (:goal (and
    
        (atribute married)
        (atribute drug_addict)
        (rank admiral)
        )
    )
)
