(define (domain sailor)
    (:requirements :strips :typing :conditional-effects :equality)
    (:types location item rank alcohol_level acquitance criminal atribute boat) 
    (:predicates
        ;Where we currently at
        (at ?where - location)
        (road ?from - location ?to - location)
        
        ;Movement
        (waterway ?from - location ?to - location)
        (item ?item - item)
        
        ;Crafting from 2 items resulting in one item
        (craft ?item1 - item ?item2 - item ?result - item)
        ;Crafting from 3 items resulting in one item
        (craft_3 ?item1 - item ?item2 - item ?item3 - item ?result - item)
        ;Crafting from 2 items at given place
        (craft_at ?where - location ?item1 - item ?result - item)
        
        ;Obtain at place withou effort
        (acquire_at ?where - location ?what - item)
        
        ;Stealable at, connected with the criminal record
        (steal_at ?where - location ?what - item)
        
        ;Investing
        (invest_at ?where - location ?from - item ?to - item)
        
        ;Shady trade connected with shady_acquitance
        (trade_at_shady ?where - location ?what - item ?for - item)
        
        ;Current rank
        (rank ?r - rank)
        ;Current alcohol level
        
        (alcohol_level ?level)
        
        ;Wheter sailor owns any type of boat
        (owns_boat)
        
        ;Determine acquitances
        (acquitance ?r - acquitance)
        
        ;Set of atributes sailor owns
        (atribute ?r - atribute)
    )

    ;Satisfy mariage goal
    (:action get_married
        :parameters ()
        :precondition (and
            (atribute girlfriend_fascinated)
            (item ring)
            (item flowers)
            (acquitance good)
            (not (atribute criminal_record))
            (at island)
            (not (alcohol_level addicted))
            (not (alcohol_level drunk))
    )
        :effect (and
            (atribute married)
        )
    )
    
    ;Satisfy admiral goal
    (:action become_admiral
        :parameters ()
        :precondition (and
        (rank captain)
        (atribute defeat_pirates)
        (at academy)
        (alcohol_level sober)
    )
        :effect (and
            (rank admiral)
        )
    )
    
    ;Satisfy drug goal
    (:action become_drug_addict
        :parameters ()
        :precondition (and
        (item cocaine)
        (alcohol_level addicted)
        (item frigate)
        (acquitance smugglers)
        (item golden_brick)
    )
        :effect (and
            (atribute drug_addict)
        )
    )
    
    ;Boat stuff---------------------------
    (:action make_boat
        :parameters ()
        :precondition (and
            (not(owns_boat))
            (item wood)
    )
        :effect (and
            (item boat)
            (not (item wood))
            (owns_boat)
        )
    )
    
    (:action steal_boat
        :parameters (?where - location)
        :precondition (and
            (not(owns_boat))
            (at ?where)
            (steal_at ?where boat)
    )
        :effect (and
            (item boat)
            (owns_boat)
            (atribute criminal_record)
        )
    )
    ;------------------------------------------------
    
    ;General actions---------------------------------
    
    ;Steals something somewhere, apply criminal record
    (:action steal_at
        :parameters (?where - location ?what - item)
        :precondition (and
            (not (item ?what))
            (at ?where)
            (steal_at ?where ?what)
    )
        :effect (and
            (item ?what)  
            (atribute criminal_record)
        )
    )
    
    ;Acquire for nothing at some place
    (:action acquire_at
        :parameters (?where - location ?what - item)
        :precondition (and
            (at ?where)
            (acquire_at ?where ?what)
    )
        :effect (and
            (item ?what)  
        )
    )
    
    (:action trade_at_shady
        :parameters (?at - location ?what - item ?for - item)
        :precondition (and
            (at ?at)
            (item ?what)
            (trade_at_shady ?at ?what ?for)
    )
        :effect (and
            (item ?for)
            (acquitance shady)
            (not (item ?what))
        )
    )
    
    
    
    (:action invest
        :parameters (?at - location ?what - item ?to - item)
        :precondition (and
            (at ?at)
            (item ?what)
            (invest_at ?at ?what ?to)
    )
        :effect (and
            (item map)
            (not (item ?what))
            (item ?to)
            (acquitance good)
        )
    )
    
    
    (:action craft
        :parameters (?from1 - item ?from2 - item ?to - item)
        :precondition (and
            (craft ?from1 ?from2 ?to)
            (item ?from1)
            (item ?from2)
    )
        :effect (and
            (not (item ?from1))
            (not (item ?from2))
            (item ?to)
        )
    )
    
    (:action craft
        :parameters (?from1 - item ?from2 - item ?from3 - item ?to - item)
        :precondition (and
            (craft_3 ?from1 ?from2 ?from3 ?to)
            (item ?from1)
            (item ?from2)
            (item ?from3)
    )
        :effect (and
            (not (item ?from1))
            (not (item ?from2))
            (not (item ?from3))
            (item ?to)
        )
    )


    (:action craft_at
        :parameters (?place - location ?from1 - item ?to - item)
        :precondition (and
            (at ?place)
            (craft_at ?place ?from1 ?to)
            (item ?from1)
    )
        :effect (and
            (not (item ?from1))

            (item ?to)
        )
    )


    (:action move
        :parameters (?from - location ?to - location)
        :precondition (and
            (at ?from)
            (road ?from ?to)
    )
        :effect (and
            (at ?to)
            (not (at ?from))
        )
    )

    (:action sail
        :parameters (?from - location ?to - location)
        :precondition (and
            (owns_boat)
            (at ?from)
            (waterway ?from ?to)
    )
        :effect (and
            (at ?to)
            (not (at ?from))
        )
    )
    
;------------------------------------------------------


;Specific actions--------------------------------------
    
    (:action cleanse_by_money
        :parameters ()
        :precondition (and
            (at town)
            (item golden_grain)
            (atribute criminal_record)
    )
        :effect (and
            (not (atribute criminal_record))
    ))
    
    
    (:action cleanse_by_work
        :parameters ()
        :precondition (and
             (at town)
             (alcohol_level sober)
             (atribute criminal_record)
    )
        :effect (and
            (not (atribute criminal_record))
            (alcohol_level mood)
            (not (alcohol_level sober))
        )
    )
    
    
    (:action study_for_captain
        :parameters ()
        :precondition (and
             (at academy)
             (not (atribute criminal_record))
             (item golden_coin)
    )
        :effect (and
            (rank captain)
            (not (item golden_coin))
        )
    )
    
    
    (:action meet_with_smuglers
        :parameters ()
        :precondition (and
            (at port)
            (acquitance shady)
            (item golden_brick)
    )
        :effect (and
            (acquitance smugglers)  
        )
    )
    
    (:action fight_with_bear
        :parameters ()
        :precondition (and
            (at woods)
    )
        :effect (and
            (atribute strong)
            (item bear_skin)
        )
    )
    
    (:action fight_in_pub
        :parameters ()
        :precondition (and
            (at pub)
            (or (alcohol_level drunk) (alcohol_level addicted) (alcohol_level mood))
    )
        :effect (and
            (atribute strong)
        )
    )
    
    (:action pirates_attack
        :parameters ()
        :precondition (and
            (at sea)
            (not (atribute strong))
    )
        :effect (and
            (not (item boat))
            (not (item frigate))
            (not (item karavela))
            (atribute strong)
            (not (owns_boat))
        )
    )

    (:action join_pirates
        :parameters ()
        :precondition (and
            (at sea)
            (acquitance shady)
    )
        :effect (and
            (alcohol_level mood)
        )
    )

    (:action defeat_pirates
        :parameters ()
        :precondition (and
            (at sea)
            (atribute strong)
            (item karavela)
    )
        :effect (and
            (alcohol_level mood)
            (atribute defeat_pirates)
            (item boat)
            (item frigate)
            (item karavela)
            (item golden_brick)
            (item golden_coin)
            (item golden_grain)
        )
    )
    
    (:action get_sober
        :parameters ()
        :precondition (and
            (or (at sea) (at river))
            (or (alcohol_level mood) (alcohol_level drunk))
    )
        :effect (and
         (not (alcohol_level mood))
         (not (alcohol_level drunk))
         (alcohol_level sober)
        )
    )
    
    
    (:action drink_a_little
        :parameters ()
        :precondition (and
            (item alcohol)
            (alcohol_level sober)
    )
        :effect (and
         (alcohol_level mood)
         (not (item alcohol))
        )
    )
    
    (:action drink_some_more
        :parameters ()
        :precondition (and
            (item alcohol)
            (alcohol_level mood)
    )
        :effect (and
         (alcohol_level drunk)
         (not (item alcohol))
        )
    )
    
    (:action get_wasted
        :parameters ()
        :precondition (and
            (item alcohol)
            (alcohol_level drunk)
    )
        :effect (and
         (alcohol_level addicted)
         (not (item alcohol))
        )
    )
    
    (:action fascinate_girlfriend
        :parameters ()
        :precondition (and
            (at lighthouse)
            (or (atribute strong) (rank captain))
    )
        :effect (and
         (atribute girlfriend_fascinated)
        )
    )
    
    
    (:action please_pub
        :parameters ()
        :precondition (and
            (at pub)
            (item golden_coin)
    )
        :effect (and
            (acquitance good)
            (not (item golden_coin))
            (item alcohol)
        )
    )
;--------------------------------------------------------

)
