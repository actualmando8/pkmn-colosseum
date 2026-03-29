    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stw r31, 0x5c(r1)
    stw r30, 0x58(r1)
    stw r29, 0x54(r1)
    mr r29, r3
    clrlwi r0, r29, 16
    cmplwi r0, 0x0
    bne @L_80051740
    li r3, 0x1
    b @L_80051E1C
    @L_80051740:
    bl fn_801FCAD0
    mr r31, r3
    clrlwi r30, r29, 16
    lwz r6, 0x0(r31)
    lwz r5, 0x4(r31)
    lwz r4, 0x8(r31)
    lwz r3, 0xc(r31)
    lwz r0, 0x10(r31)
    stw r6, 0x34(r1)
    stw r5, 0x38(r1)
    stw r4, 0x3c(r1)
    stw r3, 0x40(r1)
    stw r0, 0x44(r1)
    @L_80051774:
    mr r9, r30
    li r3, 0x8d
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    bne @L_800517E0
    li r3, 0x8d
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lwz r0, 0x34(r1)
    li r3, -0x1
    lwz r4, 0x38(r1)
    stw r0, 0x0(r31)
    lwz r0, 0x3c(r1)
    stw r4, 0x4(r31)
    lwz r4, 0x40(r1)
    stw r0, 0x8(r31)
    lwz r0, 0x44(r1)
    stw r4, 0xc(r31)
    stw r0, 0x10(r31)
    b @L_80051E1C
    @L_800517E0:
    cmpwi r3, -0x2
    bne @L_80051824
    li r3, 0x44
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    beq @L_80051810
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_80051774
    @L_80051810:
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_80051E08
    @L_80051824:
    li r3, 0x8d
    bl fn_801022B8
    cmpwi r3, 0x604
    beq @L_80051C08
    bge @L_80051864
    cmpwi r3, 0x600
    beq @L_80051988
    bge @L_80051854
    cmpwi r3, 0x5fe
    beq @L_80051888
    bge @L_80051908
    b @L_80051774
    @L_80051854:
    cmpwi r3, 0x602
    beq @L_80051B08
    bge @L_80051B88
    b @L_80051A88
    @L_80051864:
    cmpwi r3, 0x607
    beq @L_80051D88
    bge @L_8005187C
    cmpwi r3, 0x606
    bge @L_80051D08
    b @L_80051C88
    @L_8005187C:
    cmpwi r3, 0xfed
    beq @L_80051A08
    b @L_80051774
    @L_80051888:
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x1
    bl fn_801FB1C0
    addi r4, r1, 0x30
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80051900
    lwz r0, 0x30(r1)
    cmpwi r0, 0xff
    ble @L_800518D4
    li r0, 0xff
    stw r0, 0x30(r1)
    @L_800518D4:
    lwz r0, 0x30(r1)
    cmpwi r0, 0x0
    bge @L_800518E8
    li r0, 0x0
    stw r0, 0x30(r1)
    @L_800518E8:
    lwz r7, 0x30(r1)
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x1
    bl fn_801FAA58
    @L_80051900:
    bl fn_8001E200
    b @L_80051774
    @L_80051908:
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x2
    bl fn_801FB1C0
    addi r4, r1, 0x2c
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80051980
    lwz r0, 0x2c(r1)
    cmpwi r0, 0xff
    ble @L_80051954
    li r0, 0xff
    stw r0, 0x2c(r1)
    @L_80051954:
    lwz r0, 0x2c(r1)
    cmpwi r0, 0x0
    bge @L_80051968
    li r0, 0x0
    stw r0, 0x2c(r1)
    @L_80051968:
    lwz r7, 0x2c(r1)
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x2
    bl fn_801FAA58
    @L_80051980:
    bl fn_8001E200
    b @L_80051774
    @L_80051988:
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x3
    bl fn_801FB1C0
    addi r4, r1, 0x28
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80051A00
    lwz r0, 0x28(r1)
    cmpwi r0, 0xff
    ble @L_800519D4
    li r0, 0xff
    stw r0, 0x28(r1)
    @L_800519D4:
    lwz r0, 0x28(r1)
    cmpwi r0, 0x0
    bge @L_800519E8
    li r0, 0x0
    stw r0, 0x28(r1)
    @L_800519E8:
    lwz r7, 0x28(r1)
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x3
    bl fn_801FAA58
    @L_80051A00:
    bl fn_8001E200
    b @L_80051774
    @L_80051A08:
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x4
    bl fn_801FB1C0
    addi r4, r1, 0x24
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80051A80
    lwz r0, 0x24(r1)
    cmpwi r0, 0xff
    ble @L_80051A54
    li r0, 0xff
    stw r0, 0x24(r1)
    @L_80051A54:
    lwz r0, 0x24(r1)
    cmpwi r0, 0x0
    bge @L_80051A68
    li r0, 0x0
    stw r0, 0x24(r1)
    @L_80051A68:
    lwz r7, 0x24(r1)
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x4
    bl fn_801FAA58
    @L_80051A80:
    bl fn_8001E200
    b @L_80051774
    @L_80051A88:
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x5
    bl fn_801FB1C0
    addi r4, r1, 0x20
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80051B00
    lwz r0, 0x20(r1)
    cmpwi r0, 0xff
    ble @L_80051AD4
    li r0, 0xff
    stw r0, 0x20(r1)
    @L_80051AD4:
    lwz r0, 0x20(r1)
    cmpwi r0, 0x0
    bge @L_80051AE8
    li r0, 0x0
    stw r0, 0x20(r1)
    @L_80051AE8:
    lwz r7, 0x20(r1)
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x5
    bl fn_801FAA58
    @L_80051B00:
    bl fn_8001E200
    b @L_80051774
    @L_80051B08:
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x6
    bl fn_801FB1C0
    addi r4, r1, 0x1c
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80051B80
    lwz r0, 0x1c(r1)
    cmpwi r0, 0xff
    ble @L_80051B54
    li r0, 0xff
    stw r0, 0x1c(r1)
    @L_80051B54:
    lwz r0, 0x1c(r1)
    cmpwi r0, 0x0
    bge @L_80051B68
    li r0, 0x0
    stw r0, 0x1c(r1)
    @L_80051B68:
    lwz r7, 0x1c(r1)
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x6
    bl fn_801FAA58
    @L_80051B80:
    bl fn_8001E200
    b @L_80051774
    @L_80051B88:
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x7
    bl fn_801FB1C0
    addi r4, r1, 0x18
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80051C00
    lwz r0, 0x18(r1)
    cmpwi r0, 0xff
    ble @L_80051BD4
    li r0, 0xff
    stw r0, 0x18(r1)
    @L_80051BD4:
    lwz r0, 0x18(r1)
    cmpwi r0, 0x0
    bge @L_80051BE8
    li r0, 0x0
    stw r0, 0x18(r1)
    @L_80051BE8:
    lwz r7, 0x18(r1)
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x7
    bl fn_801FAA58
    @L_80051C00:
    bl fn_8001E200
    b @L_80051774
    @L_80051C08:
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x8
    bl fn_801FB1C0
    addi r4, r1, 0x14
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80051C80
    lwz r0, 0x14(r1)
    cmpwi r0, 0xff
    ble @L_80051C54
    li r0, 0xff
    stw r0, 0x14(r1)
    @L_80051C54:
    lwz r0, 0x14(r1)
    cmpwi r0, 0x0
    bge @L_80051C68
    li r0, 0x0
    stw r0, 0x14(r1)
    @L_80051C68:
    lwz r7, 0x14(r1)
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x8
    bl fn_801FAA58
    @L_80051C80:
    bl fn_8001E200
    b @L_80051774
    @L_80051C88:
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x9
    bl fn_801FB1C0
    addi r4, r1, 0x10
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80051D00
    lwz r0, 0x10(r1)
    cmpwi r0, 0xff
    ble @L_80051CD4
    li r0, 0xff
    stw r0, 0x10(r1)
    @L_80051CD4:
    lwz r0, 0x10(r1)
    cmpwi r0, 0x0
    bge @L_80051CE8
    li r0, 0x0
    stw r0, 0x10(r1)
    @L_80051CE8:
    lwz r7, 0x10(r1)
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0x9
    bl fn_801FAA58
    @L_80051D00:
    bl fn_8001E200
    b @L_80051774
    @L_80051D08:
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0xa
    bl fn_801FB1C0
    addi r4, r1, 0xc
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80051D80
    lwz r0, 0xc(r1)
    cmpwi r0, 0xff
    ble @L_80051D54
    li r0, 0xff
    stw r0, 0xc(r1)
    @L_80051D54:
    lwz r0, 0xc(r1)
    cmpwi r0, 0x0
    bge @L_80051D68
    li r0, 0x0
    stw r0, 0xc(r1)
    @L_80051D68:
    lwz r7, 0xc(r1)
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0xa
    bl fn_801FAA58
    @L_80051D80:
    bl fn_8001E200
    b @L_80051774
    @L_80051D88:
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0xb
    bl fn_801FB1C0
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80051E00
    lwz r0, 0x8(r1)
    cmpwi r0, 0xff
    ble @L_80051DD4
    li r0, 0xff
    stw r0, 0x8(r1)
    @L_80051DD4:
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    bge @L_80051DE8
    li r0, 0x0
    stw r0, 0x8(r1)
    @L_80051DE8:
    lwz r7, 0x8(r1)
    mr r4, r29
    li r3, 0x0
    li r5, 0xc
    li r6, 0xb
    bl fn_801FAA58
    @L_80051E00:
    bl fn_8001E200
    b @L_80051774
    @L_80051E08:
    li r3, 0x8d
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1
    @L_80051E1C:
    lwz r0, 0x64(r1)
    lwz r31, 0x5c(r1)
    lwz r30, 0x58(r1)
    lwz r29, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
