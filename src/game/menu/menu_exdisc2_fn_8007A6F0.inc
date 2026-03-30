    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    lwz r0, lbl_8047A638(r13)
    cmpwi r0, 0x4
    bne @L_8007A71C
    lwz r3, lbl_8047A628(r13)
    b @L_8007A72C
@L_8007A71C:
    li r3, 0x0
    li r4, 0xe
    li r5, 0x0
    bl fn_8012A5B0
@L_8007A72C:
    lwz r5, lbl_804788F0(r13)
    lis r4, lbl_802E61D8@ha
    addi r0, r4, lbl_802E61D8@l
    subi r31, r5, 0x1
    rlwinm r4, r31, 2, 0, 29
    add r4, r0, r4
    addi r0, r31, 0x1
    mtctr r0
    cmpwi r31, 0x0
    blt @L_8007A76C
@L_8007A754:
    lwz r0, 0x0(r4)
    cmplw r0, r3
    ble @L_8007A76C
    subi r4, r4, 0x4
    subi r31, r31, 0x1
    bdnz @L_8007A754
@L_8007A76C:
    cmpwi r31, 0x0
    bge @L_8007A778
    li r31, 0x0
@L_8007A778:
    mr r3, r30
    li r4, 0x0
    bl fn_80109220
    cmpwi r31, 0x2
    beq @L_8007A7C4
    bge @L_8007A79C
    cmpwi r31, 0x1
    bge @L_8007A7A8
    b @L_8007A7FC
@L_8007A79C:
    cmpwi r31, 0x4
    bge @L_8007A7FC
    b @L_8007A7E0
@L_8007A7A8:
    lha r0, 0x6(r30)
    cmpwi r0, 0x10c3
    bne @L_8007A814
    mr r3, r30
    li r4, 0x1
    bl fn_80109220
    b @L_8007A814
@L_8007A7C4:
    lha r0, 0x6(r30)
    cmpwi r0, 0x10c4
    bne @L_8007A814
    mr r3, r30
    li r4, 0x1
    bl fn_80109220
    b @L_8007A814
@L_8007A7E0:
    lha r0, 0x6(r30)
    cmpwi r0, 0x10c5
    bne @L_8007A814
    mr r3, r30
    li r4, 0x1
    bl fn_80109220
    b @L_8007A814
@L_8007A7FC:
    lha r0, 0x6(r30)
    cmpwi r0, 0x10c2
    bne @L_8007A814
    mr r3, r30
    li r4, 0x1
    bl fn_80109220
@L_8007A814:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
