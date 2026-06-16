    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_80193FF4
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_80193F7C
    lwz r3, 0x0(r30)
    bl fn_80193748
    cmplwi r3, 0x0
    bne @L_80193FB8
@L_80193F7C:
    lwz r0, lbl_8047B230(r13)
    cmplwi r0, 0x0
    beq @L_80193F90
    lwz r3, lbl_8047B230(r13)
    b @L_80193F98
@L_80193F90:
    lis r3, lbl_8036C678@ha
    addi r3, r3, lbl_8036C678@l
@L_80193F98:
    bl fn_80193828
    mr. r31, r3
    bne @L_80193FD4
    la r3, -23880(r2)
    li r4, 0x7a4
    la r5, -23872(r2)
    bl __assert
    b @L_80193FD4
@L_80193FB8:
    bl fn_80193828
    mr. r31, r3
    bne @L_80193FD4
    la r3, -23880(r2)
    li r4, 0x7f9
    la r5, -23872(r2)
    bl __assert
@L_80193FD4:
    lwz r5, 0x0(r31)
    mr r3, r31
    mr r4, r30
    lwz r12, 0x3c(r5)
    mtctr r12
    bctrl
    mr r3, r31
    b @L_80193FF8
@L_80193FF4:
    li r3, 0x0
@L_80193FF8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
