    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r28
    lwz r0, 0x18(r28)
    cmplwi r0, 0x0
    beq @L_801AA7C0
    lwz r3, 0x18(r29)
    bl fn_801C25E4
@L_801AA7C0:
    lhz r0, 0xc(r29)
    rlwinm r0, r0, 0, 18, 19
    cmpwi r0, 0x1000
    beq @L_801AA7EC
    bge @L_801AA7E0
    cmpwi r0, 0x0
    beq @L_801AA878
    b @L_801AA880
@L_801AA7E0:
    cmpwi r0, 0x2000
    beq @L_801AA824
    b @L_801AA880
@L_801AA7EC:
    lwz r31, 0x14(r29)
    cmplwi r31, 0x0
    beq @L_801AA880
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 30, 30
    beq @L_801AA80C
    lwz r3, 0x1c(r31)
    bl fn_801A6960
@L_801AA80C:
    cmplwi r31, 0x0
    beq @L_801AA880
    mr r3, r31
    li r4, 0x20
    bl fn_80193AF0
    b @L_801AA880
@L_801AA824:
    lwz r30, 0x14(r29)
    b @L_801AA86C
@L_801AA82C:
    lwz r29, 0x4(r30)
    b @L_801AA858
@L_801AA834:
    lwz r31, 0x0(r29)
    lwz r3, 0x4(r29)
    bl fn_801A053C
    cmplwi r29, 0x0
    beq @L_801AA854
    mr r3, r29
    li r4, 0xc
    bl fn_80193AF0
@L_801AA854:
    mr r29, r31
@L_801AA858:
    cmplwi r29, 0x0
    bne @L_801AA834
    mr r3, r30
    bl fn_801A3E64
    mr r30, r3
@L_801AA86C:
    cmplwi r30, 0x0
    bne @L_801AA82C
    b @L_801AA880
@L_801AA878:
    lwz r3, 0x14(r29)
    bl fn_801A053C
@L_801AA880:
    lis r4, lbl_8036CCD0@ha
    mr r3, r28
    addi r4, r4, lbl_8036CCD0@l
    lwz r4, 0x14(r4)
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
