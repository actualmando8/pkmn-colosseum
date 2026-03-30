    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    lwz r0, lbl_8047A458(r13)
    li r31, 0x0
    cmpwi r0, 0x1
    beq @L_80034734
    b @L_80034740
@L_80034734:
    lis r3, lbl_80266FA0@ha
    addi r0, r3, lbl_80266FA0@l
    mr r31, r0
@L_80034740:
    cmplwi r31, 0x0
    beq @L_8003478C
    mr r3, r30
    li r4, 0x1
    bl fn_80109220
    lha r0, 0x6(r30)
    cmpwi r0, 0x7cc
    beq @L_80034764
    b @L_800347A0
@L_80034764:
    li r3, 0xa4
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_800347A0
    lbz r0, 0x95(r3)
    extsb r0, r0
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r31, r0
    stw r0, 0x4c(r30)
    b @L_800347A0
@L_8003478C:
    li r0, 0x0
    mr r3, r30
    stw r0, 0x4c(r30)
    li r4, 0x0
    bl fn_80109220
@L_800347A0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
