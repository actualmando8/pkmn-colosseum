    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    li r31, 0x0
    li r4, 0x4
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008C624
    mr r3, r30
    li r4, 0x4
    bl fn_80121984
    extsh r0, r3
    rlwinm r0, r0, 8, 0, 23
    ori r0, r0, 0x80
    rlwinm r31, r0, 0, 16, 31
    b @L_8008C6E0
@L_8008C624:
    mr r3, r30
    li r4, 0x5
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008C648
    ori r0, r31, 0x40
    rlwinm r31, r0, 0, 16, 31
    b @L_8008C6E0
@L_8008C648:
    mr r3, r30
    li r4, 0x7
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008C66C
    ori r0, r31, 0x20
    rlwinm r31, r0, 0, 16, 31
    b @L_8008C6E0
@L_8008C66C:
    mr r3, r30
    li r4, 0x6
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008C690
    ori r0, r31, 0x10
    rlwinm r31, r0, 0, 16, 31
    b @L_8008C6E0
@L_8008C690:
    mr r3, r30
    li r4, 0x3
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008C6B4
    ori r0, r31, 0x8
    rlwinm r31, r0, 0, 16, 31
    b @L_8008C6E0
@L_8008C6B4:
    mr r3, r30
    li r4, 0x8
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008C6E0
    mr r3, r30
    li r4, 0x8
    bl fn_8012189C
    extsb r0, r3
    rlwinm r31, r0, 0, 16, 31
@L_8008C6E0:
    mr r3, r31
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
