    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lhz r31, 0x0(r3)
    lhz r30, 0x0(r4)
    mr r3, r31
    bl fn_801D1ACC
    bl fn_800FA280
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_801D1ACC
    bl fn_800FA280
    mr r0, r3
    mr r3, r29
    mr r4, r0
    bl fn_800F9EE4
    cmpwi r3, 0x0
    beq @L_8004BEEC
    b @L_8004BF04
    @L_8004BEEC:
    mr r3, r31
    bl fn_801D1E50
    mr r31, r3
    mr r3, r30
    bl fn_801D1E50
    subf r3, r31, r3
    @L_8004BF04:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
