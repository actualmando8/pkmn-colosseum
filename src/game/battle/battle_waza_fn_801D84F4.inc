    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    stw r30, 0x8(r1)
    lwz r5, 0xb0(r3)
    lwz r0, 0x8(r5)
    rlwinm r0, r0, 0, 18, 18
    cmpwi r0, 0x2000
    beq @L_801D8568
    lwz r3, 0x3c(r5)
    lbz r0, 0x18(r3)
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x1
    beq @L_801D8568
    lwz r0, 0x4(r31)
    cmpwi r0, 0x5
    beq @L_801D8568
    cmpwi r0, 0x6
    beq @L_801D8568
    cmpwi r0, 0x1
    beq @L_801D8568
    li r0, 0x2
    li r3, 0x1
    stw r0, 0x6c(r31)
    lwz r0, 0x70(r31)
    stw r0, 0x74(r31)
    b @L_801D8798
@L_801D8568:
    lwz r0, 0x4(r31)
    cmplwi r0, 0x6
    bgt @L_801D873C
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, 9740
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lwz r0, 0x78(r31)
    cmpwi r0, 0x4
    bge @L_801D85A0
    cmpwi r0, 0x0
    bge @L_801D85A8
@L_801D85A0:
    li r0, 0x0
    b @L_801D85AC
@L_801D85A8:
    li r0, 0x1
@L_801D85AC:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_801D877C
    b @L_801D8764
    mr r3, r31
    bl fn_801D91EC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D877C
    b @L_801D8764
    mr r3, r31
    bl fn_801D8B38
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D877C
    b @L_801D8764
    mr r3, r31
    bl fn_801D87B0
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D877C
    b @L_801D8764
    lwz r0, 0x7c(r31)
    li r30, 0x0
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x1
    bne @L_801D8660
    lwz r0, -21528(r13)
    cmplwi r0, 0x0
    bne @L_801D8674
    lis r4, -32746
    li r3, 0x15
    addi r8, r4, 22120
    li r5, 0x2000
    li r4, 0x4e20
    li r6, 0x1
    li r7, 0x1
    bl fn_800F07A8
    cmplwi r3, 0x0
    stw r3, -21528(r13)
    beq @L_801D8674
    lwz r5, 0x78(r31)
    li r4, 0x3
    li r6, 0x0
    li r7, 0xff
    crxor 6, 6, 6
    bl fn_800F0654
    li r30, 0x1
    b @L_801D8674
@L_801D8660:
    lwz r3, 0x78(r31)
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    mr r30, r3
@L_801D8674:
    rlwinm. r0, r30, 0, 24, 31
    bne @L_801D8764
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D877C
    li r3, 0x100
    bl fn_801D744C
    b @L_801D877C
    lwz r0, 0x78(r31)
    lwz r3, 0x3c(r5)
    cmplwi r0, 0x9
    bgt @L_801D8724
    lis r4, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r4, r4, 9700
    lwzx r0, r4, r0
    mtctr r0
    bctr
    bl fn_801DD078
    b @L_801D872C
    bl fn_801DD028
    b @L_801D872C
    li r4, 0x0
    bl fn_801DA4E8
    b @L_801D872C
    li r4, 0x1
    bl fn_801DA4E8
    b @L_801D872C
    lwz r3, 0x24(r3)
    bl fn_800E6DCC
    b @L_801D872C
    bl fn_801DCF00
    b @L_801D872C
    bl fn_801DCEA8
    b @L_801D872C
    bl fn_801DCFD8
    b @L_801D872C
    bl fn_801DCF84
    b @L_801D872C
    lwz r3, 0x6c(r3)
    cmplw r5, r3
    bne @L_801D872C
    bl fn_801DBC30
    b @L_801D872C
@L_801D8724:
    li r0, 0x0
    b @L_801D8730
@L_801D872C:
    li r0, 0x1
@L_801D8730:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_801D877C
    b @L_801D8764
@L_801D873C:
    lis r3, -32728
    addi r3, r3, -27164
    crxor 6, 6, 6
    bl fn_800DD970
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D877C
    li r3, 0x1
    bl fn_801D744C
    b @L_801D877C
@L_801D8764:
    li r0, 0x1
    li r3, 0x1
    stw r0, 0x6c(r31)
    lwz r0, 0x70(r31)
    stw r0, 0x74(r31)
    b @L_801D8798
@L_801D877C:
    lis r3, -32728
    addi r3, r3, -27120
    crxor 6, 6, 6
    bl fn_800DD970
    li r0, 0x2
    li r3, 0x0
    stw r0, 0x6c(r31)
@L_801D8798:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
