    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x4c
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80011738
    mr r5, r31
    li r3, 0x4c
    li r4, 0x0
    bl fn_80102568
    @L_80011738:
    li r3, 0xf9
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8001175C
    mr r5, r31
    li r3, 0xf9
    li r4, 0x0
    bl fn_80102568
    @L_8001175C:
    li r3, 0xfa
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80011780
    mr r5, r31
    li r3, 0xfa
    li r4, 0x0
    bl fn_80102568
    @L_80011780:
    li r3, 0xf7
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800117A4
    mr r5, r31
    li r3, 0xf7
    li r4, 0x0
    bl fn_80102568
    @L_800117A4:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
