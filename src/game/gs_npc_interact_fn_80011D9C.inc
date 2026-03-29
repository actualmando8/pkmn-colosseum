    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    li r30, 0x0
    bl fn_80104704
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_80011E48
    cmpwi r28, 0x49
    beq @L_80011E00
    bge @L_80011DF4
    cmpwi r28, 0x47
    bge @L_80011E08
    cmpwi r28, 0x45
    bge @L_80011E00
    b @L_80011E0C
    @L_80011DF4:
    cmpwi r28, 0x4b
    bge @L_80011E0C
    b @L_80011E08
    @L_80011E00:
    li r30, 0x538
    b @L_80011E0C
    @L_80011E08:
    li r30, 0x540
    @L_80011E0C:
    cmpwi r29, 0x0
    beq @L_80011E38
    mr r3, r31
    mr r4, r30
    li r5, 0x1
    bl fn_80103F74
    mr r3, r31
    mr r4, r30
    li r5, 0x2d
    bl fn_801081F8
    b @L_80011E48
    @L_80011E38:
    mr r3, r31
    mr r4, r30
    li r5, 0x0
    bl fn_80103F74
    @L_80011E48:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
