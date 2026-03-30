    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr. r31, r3
    stw r30, 0x18(r1)
    beq @L_801A7CE0
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    beq @L_801A7B5C
    lwz r3, 0x0(r31)
    bl fn_80193748
    cmplwi r3, 0x0
    bne @L_801A7B98
    @L_801A7B5C:
    lwz r0, lbl_8047B2D0(r13)
    cmplwi r0, 0x0
    beq @L_801A7B70
    lwz r3, lbl_8047B2D0(r13)
    b @L_801A7B78
    @L_801A7B70:
    lis r3, lbl_8036CB30@ha
    addi r3, r3, lbl_8036CB30@l
    @L_801A7B78:
    bl fn_80193828
    mr. r30, r3
    bne @L_801A7BB4
    la r3, lbl_8047DC18(r2)
    li r4, 0x44a
    la r5, lbl_8047DC30(r2)
    bl fn_80196E10
    b @L_801A7BB4
    @L_801A7B98:
    bl fn_80193828
    mr. r30, r3
    bne @L_801A7BB4
    la r3, lbl_8047DC18(r2)
    li r4, 0x175
    la r5, lbl_8047DC30(r2)
    bl fn_80196E10
    @L_801A7BB4:
    lwz r5, 0x0(r30)
    mr r3, r30
    mr r4, r31
    lwz r12, 0x40(r5)
    mtctr r12
    bctrl
    cmplwi r30, 0x0
    li r31, 0x0
    beq @L_801A7CD8
    lwz r0, 0x18(r30)
    cmplwi r0, 0x0
    beq @L_801A7BF4
    lwz r3, 0x18(r30)
    bl fn_801B42C0
    li r0, 0x0
    stw r0, 0x18(r30)
    @L_801A7BF4:
    lwz r0, 0x1c(r30)
    cmplwi r0, 0x0
    beq @L_801A7C18
    lwz r3, 0x1c(r30)
    li r4, 0x7
    li r5, 0x1
    bl fn_801B7178
    li r0, 0x0
    stw r0, 0x1c(r30)
    @L_801A7C18:
    lwz r0, 0x8(r30)
    stw r0, 0x8(r1)
    lwz r0, 0x4(r30)
    rlwinm. r0, r0, 0, 5, 5
    beq @L_801A7C5C
    lwz r0, lbl_8047B2DC(r13)
    cmplwi r0, 0x0
    beq @L_801A7C5C
    addi r31, r1, 0x8
    b @L_801A7C48
    @L_801A7C40:
    lwz r3, 0x0(r31)
    addi r31, r3, 0x8
    @L_801A7C48:
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_801A7C40
    lwz r0, lbl_8047B2DC(r13)
    stw r0, 0x0(r31)
    @L_801A7C5C:
    lwz r0, 0x4(r30)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_801A7C98
    lwz r0, lbl_8047B2D8(r13)
    cmplwi r0, 0x0
    beq @L_801A7C98
    lwz r3, lbl_8047B2D8(r13)
    lwz r0, 0x58(r3)
    cmplwi r0, 0x0
    beq @L_801A7C98
    lwz r0, 0x8(r1)
    lwz r3, lbl_8047B2D8(r13)
    stw r0, 0x8(r3)
    lwz r0, lbl_8047B2D8(r13)
    stw r0, 0x8(r1)
    @L_801A7C98:
    lwz r3, 0x8(r1)
    bl fn_801BC33C
    lwz r4, 0x0(r30)
    mr r3, r30
    addi r5, r30, 0x1c
    lwz r12, 0x44(r4)
    lwz r4, 0x8(r1)
    mtctr r12
    bctrl
    addi r4, r30, 0x18
    addi r5, r30, 0x1c
    bl fn_801B4300
    cmplwi r31, 0x0
    beq @L_801A7CD8
    li r0, 0x0
    stw r0, 0x0(r31)
    @L_801A7CD8:
    mr r3, r30
    b @L_801A7CE4
    @L_801A7CE0:
    li r3, 0x0
    @L_801A7CE4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
