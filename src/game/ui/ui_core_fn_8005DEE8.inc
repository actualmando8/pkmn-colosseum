    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r0, lbl_8047A5C0(r13)
    cmpwi r0, 0x0
    bne @L_8005DF10
    li r0, 0x1
    stw r0, lbl_8047A5C8(r13)
    stw r0, lbl_8047A5C0(r13)
@L_8005DF10:
    li r0, 0x1
    stw r0, lbl_8047A5C4(r13)
@L_8005DF18:
    li r3, 0xcb
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, -0x1
    beq @L_8005DFA0
    lwz r0, lbl_8047A5C8(r13)
    clrlwi r3, r0, 16
    bl fn_80142984
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8005DF18
    lwz r0, lbl_8047A5C4(r13)
    cmpwi r0, 0x1
    blt @L_8005DF18
    cmpwi r0, 0x3e7
    bgt @L_8005DF18
    li r3, 0x44
    li r4, 0x1
    bl fn_8010264C
    mr r31, r3
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    cmpwi r31, 0x0
    bne @L_8005DF18
    lwz r4, lbl_8047A5C8(r13)
    li r3, 0x0
    lwz r0, lbl_8047A5C4(r13)
    li r6, -0x1
    clrlwi r4, r4, 16
    clrlwi r5, r0, 16
    bl fn_80129A78
    b @L_8005DF18
@L_8005DFA0:
    li r3, 0xcb
    bl fn_80102510
    li r3, 0xcb
    li r4, 0x1
    bl fn_80102428
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
