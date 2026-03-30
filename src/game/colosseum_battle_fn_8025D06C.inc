    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_8006B5A8
    bl fn_8006AFC4
    bl fn_8006ADEC
    mr r0, r3
    li r3, 0x0
    mr r4, r0
    bl fn_801293FC
    lwz r0, 0x14(r1)
    li r3, 0x0
    mtlr r0
    addi r1, r1, 0x10
    blr
