    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    b @L_8000C754
    @L_8000C700:
    lwz r4, lbl_80478E18(r13)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    bge @L_8000C754
    lwz r4, lbl_80478E1C(r13)
    slwi r0, r3, 1
    lhzx r31, r4, r0
    mr r3, r31
    bl fn_801666BC
    cmpwi r3, 0x0
    beq @L_8000C744
    blt @L_8000C744
    cmpwi r3, 0x4
    bge @L_8000C744
    mr r3, r31
    bl fn_80166B18
    b @L_8000C754
    @L_8000C744:
    mr r3, r31
    li r4, 0x0
    li r5, 0x7f
    bl fn_801659FC
    @L_8000C754:
    li r3, 0x2
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, -0x1
    bne @L_8000C700
    li r3, 0x2
    bl fn_80102510
    li r3, -0x1
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
