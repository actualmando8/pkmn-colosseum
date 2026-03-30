    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r30, r4
    cmplwi r3, 0x0
    bne @L_8007D590
    li r3, 0xa6
    bl fn_80104704
@L_8007D590:
    bl fn_801040A0
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_8007D780
    lha r0, 0x6(r30)
    cmpwi r0, 0x797
    beq @L_8007D704
    bge @L_8007D5C8
    cmpwi r0, 0x793
    beq @L_8007D6CC
    blt @L_8007D738
    cmpwi r0, 0x796
    bge @L_8007D694
    b @L_8007D738
@L_8007D5C8:
    cmpwi r0, 0x1194
    beq @L_8007D65C
    bge @L_8007D5E0
    cmpwi r0, 0x1193
    bge @L_8007D5EC
    b @L_8007D738
@L_8007D5E0:
    cmpwi r0, 0x1196
    bge @L_8007D738
    b @L_8007D624
@L_8007D5EC:
    lwz r0, 0xac(r3)
    li r31, 0x0
    cmpwi r0, 0x0
    ble @L_8007D608
    lwz r0, 0xa0(r3)
    cmpwi r0, 0x0
    bge @L_8007D610
@L_8007D608:
    li r0, 0x0
    b @L_8007D61C
@L_8007D610:
    lwz r3, 0xb0(r3)
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
@L_8007D61C:
    mr r29, r0
    b @L_8007D738
@L_8007D624:
    lwz r0, 0xac(r3)
    li r31, 0x1
    cmpwi r0, 0x0
    ble @L_8007D640
    lwz r0, 0xa0(r3)
    cmpwi r0, 0x0
    bge @L_8007D648
@L_8007D640:
    li r0, 0x0
    b @L_8007D654
@L_8007D648:
    lwz r3, 0xb0(r3)
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
@L_8007D654:
    mr r29, r0
    b @L_8007D738
@L_8007D65C:
    lwz r0, 0xac(r3)
    li r31, 0x2
    cmpwi r0, 0x0
    ble @L_8007D678
    lwz r0, 0xa0(r3)
    cmpwi r0, 0x0
    bge @L_8007D680
@L_8007D678:
    li r0, 0x0
    b @L_8007D68C
@L_8007D680:
    lwz r3, 0xb0(r3)
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
@L_8007D68C:
    mr r29, r0
    b @L_8007D738
@L_8007D694:
    lwz r0, 0xac(r3)
    li r31, 0x0
    cmpwi r0, 0x0
    ble @L_8007D6B0
    lwz r0, 0xa4(r3)
    cmpwi r0, 0x0
    bge @L_8007D6B8
@L_8007D6B0:
    li r0, 0x0
    b @L_8007D6C4
@L_8007D6B8:
    lwz r3, 0xb0(r3)
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
@L_8007D6C4:
    mr r29, r0
    b @L_8007D738
@L_8007D6CC:
    lwz r0, 0xac(r3)
    li r31, 0x1
    cmpwi r0, 0x0
    ble @L_8007D6E8
    lwz r0, 0xa4(r3)
    cmpwi r0, 0x0
    bge @L_8007D6F0
@L_8007D6E8:
    li r0, 0x0
    b @L_8007D6FC
@L_8007D6F0:
    lwz r3, 0xb0(r3)
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
@L_8007D6FC:
    mr r29, r0
    b @L_8007D738
@L_8007D704:
    lwz r0, 0xac(r3)
    li r31, 0x2
    cmpwi r0, 0x0
    ble @L_8007D720
    lwz r0, 0xa4(r3)
    cmpwi r0, 0x0
    bge @L_8007D728
@L_8007D720:
    li r0, 0x0
    b @L_8007D734
@L_8007D728:
    lwz r3, 0xb0(r3)
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
@L_8007D734:
    mr r29, r0
@L_8007D738:
    cmplwi r29, 0x0
    beq @L_8007D778
    lbz r0, 0x1b(r29)
    extsb r3, r31
    extsb r0, r0
    cmpw r3, r0
    bge @L_8007D778
    mr r3, r29
    mr r4, r31
    bl fn_80082FE4
    li r0, 0xe4
    mr r4, r3
    stw r0, 0x4c(r30)
    li r3, 0x37
    bl fn_80132A38
    b @L_8007D780
@L_8007D778:
    li r0, 0x0
    stw r0, 0x4c(r30)
@L_8007D780:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
