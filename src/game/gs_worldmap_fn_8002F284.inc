    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    li r4, 0x0
    lis r3, lbl_803A2688@ha
    addi r0, r3, lbl_803A2688@l
    stw r4, 0x8(r1)
    mr r31, r0
    li r3, 0x1005
    li r4, 0x0
    bl fn_8005D8F8
    li r3, 0x1002
    li r4, 0x0
    bl fn_8005D8F8
    li r3, 0x1004
    li r4, 0x0
    bl fn_8005D8F8
    li r3, 0x1001
    li r4, 0x0
    bl fn_8005D8F8
    li r3, 0x1003
    li r4, 0x0
    bl fn_8005D8F8
    li r3, 0x1000
    li r4, 0x0
    bl fn_8005D8F8
    lis r3, lbl_803A2688@ha
    li r4, 0x0
    addi r3, r3, lbl_803A2688@l
    bl fn_8012AC08
    mr r30, r3
    bl fn_8011E850
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002F320
    li r4, 0x1
    b @L_8002F354
    @L_8002F320:
    mr r3, r30
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002F350
    mr r3, r30
    bl fn_80075FEC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8002F350
    li r4, 0x1
    b @L_8002F354
    @L_8002F350:
    li r4, 0x0
    @L_8002F354:
    li r3, 0xfff
    bl fn_8005D8F8
    mr r3, r31
    li r4, 0x1
    bl fn_8012AC08
    mr r30, r3
    bl fn_8011E850
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002F384
    li r4, 0x1
    b @L_8002F3B8
    @L_8002F384:
    mr r3, r30
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002F3B4
    mr r3, r30
    bl fn_80075FEC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8002F3B4
    li r4, 0x1
    b @L_8002F3B8
    @L_8002F3B4:
    li r4, 0x0
    @L_8002F3B8:
    li r3, 0xffc
    bl fn_8005D8F8
    mr r3, r31
    li r4, 0x2
    bl fn_8012AC08
    mr r30, r3
    bl fn_8011E850
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002F3E8
    li r4, 0x1
    b @L_8002F41C
    @L_8002F3E8:
    mr r3, r30
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002F418
    mr r3, r30
    bl fn_80075FEC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8002F418
    li r4, 0x1
    b @L_8002F41C
    @L_8002F418:
    li r4, 0x0
    @L_8002F41C:
    li r3, 0xffe
    bl fn_8005D8F8
    mr r3, r31
    li r4, 0x3
    bl fn_8012AC08
    mr r30, r3
    bl fn_8011E850
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002F44C
    li r4, 0x1
    b @L_8002F480
    @L_8002F44C:
    mr r3, r30
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002F47C
    mr r3, r30
    bl fn_80075FEC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8002F47C
    li r4, 0x1
    b @L_8002F480
    @L_8002F47C:
    li r4, 0x0
    @L_8002F480:
    li r3, 0xffb
    bl fn_8005D8F8
    mr r3, r31
    li r4, 0x4
    bl fn_8012AC08
    mr r30, r3
    bl fn_8011E850
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002F4B0
    li r4, 0x1
    b @L_8002F4E4
    @L_8002F4B0:
    mr r3, r30
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002F4E0
    mr r3, r30
    bl fn_80075FEC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8002F4E0
    li r4, 0x1
    b @L_8002F4E4
    @L_8002F4E0:
    li r4, 0x0
    @L_8002F4E4:
    li r3, 0xffd
    bl fn_8005D8F8
    mr r3, r31
    li r4, 0x5
    bl fn_8012AC08
    mr r30, r3
    bl fn_8011E850
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002F514
    li r4, 0x1
    b @L_8002F548
    @L_8002F514:
    mr r3, r30
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002F544
    mr r3, r30
    bl fn_80075FEC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8002F544
    li r4, 0x1
    b @L_8002F548
    @L_8002F544:
    li r4, 0x0
    @L_8002F548:
    li r3, 0xffa
    bl fn_8005D8F8
    lbz r0, lbl_8047A410(r13)
    cmplwi r0, 0x0
    bne @L_8002F56C
    li r3, 0xd9
    bl fn_801023E4
    cmpwi r3, 0x0
    bne @L_8002F584
    @L_8002F56C:
    li r3, 0xd9
    li r4, 0xfff
    bl fn_80102138
    li r0, 0x0
    stw r3, 0x8(r1)
    stb r0, lbl_8047A410(r13)
    @L_8002F584:
    li r3, 0xd9
    li r4, 0x1
    bl fn_801021F8
    li r3, 0x2
    bl fn_80103CC0
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    beq @L_8002F5CC
    bl fn_801046B8
    mr r4, r3
    addi r5, r1, 0x8
    li r3, 0xd9
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    b @L_8002F5F0
    @L_8002F5CC:
    bl fn_801046B8
    mr r4, r3
    li r3, 0xd9
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    @L_8002F5F0:
    li r3, 0xd9
    bl fn_80104704
    mr r31, r3
    li r4, 0x10b2
    bl fn_801046C8
    cmplwi r31, 0x0
    mr r31, r3
    beq @L_8002F628
    cmplwi r31, 0x0
    beq @L_8002F628
    li r4, 0x1
    bl fn_80109220
    li r0, 0x43d9
    stw r0, 0x4c(r31)
    @L_8002F628:
    li r3, 0xd9
    li r4, 0x1
    bl fn_801045A8
    li r3, 0x1
    bl fn_80103CC0
    bl fn_80102004
    cmpwi r3, 0x1
    bne @L_8002F670
    li r3, 0x2
    li r4, 0x4448
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0xd9
    bl fn_80102510
    li r0, 0x0
    stw r0, lbl_8047A42C(r13)
    b @L_8002F784
    @L_8002F670:
    li r3, 0xd9
    bl fn_801043A4
    mr r31, r3
    li r30, 0x0
    li r3, 0xd9
    bl fn_801022B8
    lis r4, lbl_80266E90@ha
    li r5, 0x0
    addi r4, r4, lbl_80266E90@l
    li r0, 0x2
    mtctr r0
    @L_8002F69C:
    lhz r0, 0x10(r4)
    cmpw r3, r0
    bne @L_8002F6AC
    lbz r30, 0x1(r4)
    @L_8002F6AC:
    addi r4, r4, 0x12
    lhz r0, 0x10(r4)
    cmpw r3, r0
    bne @L_8002F6C0
    lbz r30, 0x1(r4)
    @L_8002F6C0:
    addi r4, r4, 0x12
    lhz r0, 0x10(r4)
    cmpw r3, r0
    bne @L_8002F6D4
    lbz r30, 0x1(r4)
    @L_8002F6D4:
    addi r4, r4, 0x12
    lhz r0, 0x10(r4)
    cmpw r3, r0
    bne @L_8002F6E8
    lbz r30, 0x1(r4)
    @L_8002F6E8:
    addi r4, r4, 0x12
    lhz r0, 0x10(r4)
    cmpw r3, r0
    bne @L_8002F6FC
    lbz r30, 0x1(r4)
    @L_8002F6FC:
    addi r4, r4, 0x12
    lhz r0, 0x10(r4)
    cmpw r3, r0
    bne @L_8002F710
    lbz r30, 0x1(r4)
    @L_8002F710:
    addi r4, r4, 0x12
    addi r5, r5, 0x5
    bdnz @L_8002F69C
    li r3, 0xd9
    bl fn_801022B8
    cmpwi r3, 0xff9
    bne @L_8002F730
    li r30, 0x3e8
    @L_8002F730:
    cmpwi r31, -0x1
    mr r3, r30
    bne @L_8002F740
    li r3, -0x1
    @L_8002F740:
    li r0, -0x1
    cmpwi r3, 0x3e8
    stw r0, lbl_8047A428(r13)
    beq @L_8002F76C
    bge @L_8002F778
    cmpwi r3, -0x1
    beq @L_8002F760
    b @L_8002F778
    @L_8002F760:
    li r0, 0x9
    stw r0, lbl_8047A42C(r13)
    b @L_8002F784
    @L_8002F76C:
    li r0, 0x9
    stw r0, lbl_8047A42C(r13)
    b @L_8002F784
    @L_8002F778:
    li r0, 0x8
    stw r3, lbl_8047A428(r13)
    stw r0, lbl_8047A42C(r13)
    @L_8002F784:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
