    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r7, lbl_80478B08(r13)
    li r5, 0x0
    lhz r6, 0x0(r7)
    b @L_800FBDD8
@L_800FBDA8:
    clrlwi r0, r5, 24
    lwz r4, 0x20(r7)
    mulli r0, r0, 0x68
    addi r5, r5, 0x1
    add r31, r4, r0
    lbz r0, 0x0(r31)
    cmplwi r0, 0x0
    beq @L_800FBDD8
    lwz r0, 0x1c(r31)
    cmplw r0, r3
    bne @L_800FBDD8
    b @L_800FBDE8
@L_800FBDD8:
    clrlwi r0, r5, 24
    cmpw r0, r6
    blt @L_800FBDA8
    li r31, 0x0
@L_800FBDE8:
    cmplwi r31, 0x0
    beq @L_800FBE68
    lbz r0, 0x3(r31)
    li r3, 0x0
    cmpwi r0, 0x3
    beq @L_800FBE40
    bge @L_800FBE1C
    cmpwi r0, 0x1
    beq @L_800FBE30
    bge @L_800FBE38
    cmpwi r0, 0x0
    bge @L_800FBE54
    b @L_800FBE54
@L_800FBE1C:
    cmpwi r0, 0x5
    beq @L_800FBE50
    bge @L_800FBE54
    b @L_800FBE48
    b @L_800FBE54
@L_800FBE30:
    li r3, 0x57
    b @L_800FBE54
@L_800FBE38:
    li r3, 0x58
    b @L_800FBE54
@L_800FBE40:
    li r3, 0x59
    b @L_800FBE54
@L_800FBE48:
    li r3, 0x497
    b @L_800FBE54
@L_800FBE50:
    li r3, 0x498
@L_800FBE54:
    cmplwi r3, 0x0
    beq @L_800FBE60
    bl fn_801669BC
@L_800FBE60:
    li r0, 0x0
    stb r0, 0x0(r31)
@L_800FBE68:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
