    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stw r31, 0x5c(r1)
    stw r30, 0x58(r1)
    li r30, 0x0
    lis r3, lbl_803A6AB0@ha
    addi r31, r3, lbl_803A6AB0@l
    b @L_8004EE30
    @L_8004EDF0:
    mr r3, r30
    bl fn_801FBFBC
    clrlwi r0, r30, 16
    lwz r4, 0x0(r3)
    mulli r5, r0, 0x14
    lwz r0, 0x4(r3)
    addi r30, r30, 0x1
    add r5, r31, r5
    stw r4, 0x0(r5)
    stw r0, 0x4(r5)
    lwz r4, 0x8(r3)
    lwz r0, 0xc(r3)
    stw r4, 0x8(r5)
    stw r0, 0xc(r5)
    lwz r0, 0x10(r3)
    stw r0, 0x10(r5)
    @L_8004EE30:
    clrlwi r0, r30, 16
    cmplwi r0, 0x229
    blt @L_8004EDF0
    @L_8004EE3C:
    li r3, 0x8c
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    bne @L_8004EED8
    li r3, 0x8c
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r30, 0x0
    lis r3, lbl_803A6AB0@ha
    addi r31, r3, lbl_803A6AB0@l
    b @L_8004EEC4
    @L_8004EE84:
    mr r3, r30
    bl fn_801FBFBC
    clrlwi r0, r30, 16
    addi r30, r30, 0x1
    mulli r0, r0, 0x14
    add r5, r31, r0
    lwz r4, 0x0(r5)
    lwz r0, 0x4(r5)
    stw r4, 0x0(r3)
    stw r0, 0x4(r3)
    lwz r4, 0x8(r5)
    lwz r0, 0xc(r5)
    stw r4, 0x8(r3)
    stw r0, 0xc(r3)
    lwz r0, 0x10(r5)
    stw r0, 0x10(r3)
    @L_8004EEC4:
    clrlwi r0, r30, 16
    cmplwi r0, 0x229
    blt @L_8004EE84
    li r3, -0x1
    b @L_8004F848
    @L_8004EED8:
    cmpwi r3, -0x2
    bne @L_8004EF1C
    li r3, 0x44
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    beq @L_8004EF08
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_8004EE3C
    @L_8004EF08:
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_8004F834
    @L_8004EF1C:
    li r3, 0x8c
    bl fn_801022B8
    cmpwi r3, 0x5f1
    beq @L_8004F4B4
    bge @L_8004EF78
    cmpwi r3, 0x5ec
    beq @L_8004F234
    bge @L_8004EF60
    cmpwi r3, 0x5e9
    beq @L_8004F034
    bge @L_8004EF54
    cmpwi r3, 0x5e8
    bge @L_8004EFB4
    b @L_8004EE3C
    @L_8004EF54:
    cmpwi r3, 0x5eb
    bge @L_8004F1B4
    b @L_8004F134
    @L_8004EF60:
    cmpwi r3, 0x5ef
    beq @L_8004F3B4
    bge @L_8004F434
    cmpwi r3, 0x5ee
    bge @L_8004F334
    b @L_8004F2B4
    @L_8004EF78:
    cmpwi r3, 0x5f6
    beq @L_8004F734
    bge @L_8004EF9C
    cmpwi r3, 0x5f4
    beq @L_8004F634
    bge @L_8004F6B4
    cmpwi r3, 0x5f3
    bge @L_8004F5B4
    b @L_8004F534
    @L_8004EF9C:
    cmpwi r3, 0xef0
    beq @L_8004F0B4
    bge @L_8004EE3C
    cmpwi r3, 0x5f8
    bge @L_8004EE3C
    b @L_8004F7B4
    @L_8004EFB4:
    li r3, 0x0
    li r4, 0x3c
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x48
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F02C
    lwz r0, 0x48(r1)
    cmpwi r0, 0xc8
    ble @L_8004F000
    li r0, 0xc8
    stw r0, 0x48(r1)
    @L_8004F000:
    lwz r0, 0x48(r1)
    cmpwi r0, -0xc8
    bge @L_8004F014
    li r0, -0xc8
    stw r0, 0x48(r1)
    @L_8004F014:
    lwz r7, 0x48(r1)
    li r3, 0x0
    li r4, 0x3c
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F02C:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F034:
    li r3, 0x0
    li r4, 0x3d
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x44
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F0AC
    lwz r0, 0x44(r1)
    cmpwi r0, 0xc8
    ble @L_8004F080
    li r0, 0xc8
    stw r0, 0x44(r1)
    @L_8004F080:
    lwz r0, 0x44(r1)
    cmpwi r0, -0xc8
    bge @L_8004F094
    li r0, -0xc8
    stw r0, 0x44(r1)
    @L_8004F094:
    lwz r7, 0x44(r1)
    li r3, 0x0
    li r4, 0x3d
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F0AC:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F0B4:
    li r3, 0x0
    li r4, 0x3e
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x40
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F12C
    lwz r0, 0x40(r1)
    cmpwi r0, 0xc8
    ble @L_8004F100
    li r0, 0xc8
    stw r0, 0x40(r1)
    @L_8004F100:
    lwz r0, 0x40(r1)
    cmpwi r0, -0xc8
    bge @L_8004F114
    li r0, -0xc8
    stw r0, 0x40(r1)
    @L_8004F114:
    lwz r7, 0x40(r1)
    li r3, 0x0
    li r4, 0x3e
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F12C:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F134:
    li r3, 0x0
    li r4, 0x3f
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x3c
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F1AC
    lwz r0, 0x3c(r1)
    cmpwi r0, 0xc8
    ble @L_8004F180
    li r0, 0xc8
    stw r0, 0x3c(r1)
    @L_8004F180:
    lwz r0, 0x3c(r1)
    cmpwi r0, -0xc8
    bge @L_8004F194
    li r0, -0xc8
    stw r0, 0x3c(r1)
    @L_8004F194:
    lwz r7, 0x3c(r1)
    li r3, 0x0
    li r4, 0x3f
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F1AC:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F1B4:
    li r3, 0x0
    li r4, 0x40
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x38
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F22C
    lwz r0, 0x38(r1)
    cmpwi r0, 0xc8
    ble @L_8004F200
    li r0, 0xc8
    stw r0, 0x38(r1)
    @L_8004F200:
    lwz r0, 0x38(r1)
    cmpwi r0, -0xc8
    bge @L_8004F214
    li r0, -0xc8
    stw r0, 0x38(r1)
    @L_8004F214:
    lwz r7, 0x38(r1)
    li r3, 0x0
    li r4, 0x40
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F22C:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F234:
    li r3, 0x0
    li r4, 0x44
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x34
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F2AC
    lwz r0, 0x34(r1)
    cmpwi r0, 0xc8
    ble @L_8004F280
    li r0, 0xc8
    stw r0, 0x34(r1)
    @L_8004F280:
    lwz r0, 0x34(r1)
    cmpwi r0, -0xc8
    bge @L_8004F294
    li r0, -0xc8
    stw r0, 0x34(r1)
    @L_8004F294:
    lwz r7, 0x34(r1)
    li r3, 0x0
    li r4, 0x44
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F2AC:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F2B4:
    li r3, 0x0
    li r4, 0x48
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x30
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F32C
    lwz r0, 0x30(r1)
    cmpwi r0, 0xc8
    ble @L_8004F300
    li r0, 0xc8
    stw r0, 0x30(r1)
    @L_8004F300:
    lwz r0, 0x30(r1)
    cmpwi r0, -0xc8
    bge @L_8004F314
    li r0, -0xc8
    stw r0, 0x30(r1)
    @L_8004F314:
    lwz r7, 0x30(r1)
    li r3, 0x0
    li r4, 0x48
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F32C:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F334:
    li r3, 0x0
    li r4, 0x49
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x2c
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F3AC
    lwz r0, 0x2c(r1)
    cmpwi r0, 0xc8
    ble @L_8004F380
    li r0, 0xc8
    stw r0, 0x2c(r1)
    @L_8004F380:
    lwz r0, 0x2c(r1)
    cmpwi r0, -0xc8
    bge @L_8004F394
    li r0, -0xc8
    stw r0, 0x2c(r1)
    @L_8004F394:
    lwz r7, 0x2c(r1)
    li r3, 0x0
    li r4, 0x49
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F3AC:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F3B4:
    li r3, 0x0
    li r4, 0x4b
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x28
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F42C
    lwz r0, 0x28(r1)
    cmpwi r0, 0xc8
    ble @L_8004F400
    li r0, 0xc8
    stw r0, 0x28(r1)
    @L_8004F400:
    lwz r0, 0x28(r1)
    cmpwi r0, -0xc8
    bge @L_8004F414
    li r0, -0xc8
    stw r0, 0x28(r1)
    @L_8004F414:
    lwz r7, 0x28(r1)
    li r3, 0x0
    li r4, 0x4b
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F42C:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F434:
    li r3, 0x0
    li r4, 0x4c
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x24
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F4AC
    lwz r0, 0x24(r1)
    cmpwi r0, 0xc8
    ble @L_8004F480
    li r0, 0xc8
    stw r0, 0x24(r1)
    @L_8004F480:
    lwz r0, 0x24(r1)
    cmpwi r0, -0xc8
    bge @L_8004F494
    li r0, -0xc8
    stw r0, 0x24(r1)
    @L_8004F494:
    lwz r7, 0x24(r1)
    li r3, 0x0
    li r4, 0x4c
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F4AC:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F4B4:
    li r3, 0x0
    li r4, 0x4a
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x20
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F52C
    lwz r0, 0x20(r1)
    cmpwi r0, 0xc8
    ble @L_8004F500
    li r0, 0xc8
    stw r0, 0x20(r1)
    @L_8004F500:
    lwz r0, 0x20(r1)
    cmpwi r0, -0xc8
    bge @L_8004F514
    li r0, -0xc8
    stw r0, 0x20(r1)
    @L_8004F514:
    lwz r7, 0x20(r1)
    li r3, 0x0
    li r4, 0x4a
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F52C:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F534:
    li r3, 0x0
    li r4, 0x45
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x1c
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F5AC
    lwz r0, 0x1c(r1)
    cmpwi r0, 0xc8
    ble @L_8004F580
    li r0, 0xc8
    stw r0, 0x1c(r1)
    @L_8004F580:
    lwz r0, 0x1c(r1)
    cmpwi r0, -0xc8
    bge @L_8004F594
    li r0, -0xc8
    stw r0, 0x1c(r1)
    @L_8004F594:
    lwz r7, 0x1c(r1)
    li r3, 0x0
    li r4, 0x45
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F5AC:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F5B4:
    li r3, 0x0
    li r4, 0x46
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x18
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F62C
    lwz r0, 0x18(r1)
    cmpwi r0, 0xc8
    ble @L_8004F600
    li r0, 0xc8
    stw r0, 0x18(r1)
    @L_8004F600:
    lwz r0, 0x18(r1)
    cmpwi r0, -0xc8
    bge @L_8004F614
    li r0, -0xc8
    stw r0, 0x18(r1)
    @L_8004F614:
    lwz r7, 0x18(r1)
    li r3, 0x0
    li r4, 0x46
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F62C:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F634:
    li r3, 0x0
    li r4, 0x47
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x14
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F6AC
    lwz r0, 0x14(r1)
    cmpwi r0, 0xc8
    ble @L_8004F680
    li r0, 0xc8
    stw r0, 0x14(r1)
    @L_8004F680:
    lwz r0, 0x14(r1)
    cmpwi r0, -0xc8
    bge @L_8004F694
    li r0, -0xc8
    stw r0, 0x14(r1)
    @L_8004F694:
    lwz r7, 0x14(r1)
    li r3, 0x0
    li r4, 0x47
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F6AC:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F6B4:
    li r3, 0x0
    li r4, 0x41
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x10
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F72C
    lwz r0, 0x10(r1)
    cmpwi r0, 0xc8
    ble @L_8004F700
    li r0, 0xc8
    stw r0, 0x10(r1)
    @L_8004F700:
    lwz r0, 0x10(r1)
    cmpwi r0, -0xc8
    bge @L_8004F714
    li r0, -0xc8
    stw r0, 0x10(r1)
    @L_8004F714:
    lwz r7, 0x10(r1)
    li r3, 0x0
    li r4, 0x41
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F72C:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F734:
    li r3, 0x0
    li r4, 0x42
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0xc
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F7AC
    lwz r0, 0xc(r1)
    cmpwi r0, 0xc8
    ble @L_8004F780
    li r0, 0xc8
    stw r0, 0xc(r1)
    @L_8004F780:
    lwz r0, 0xc(r1)
    cmpwi r0, -0xc8
    bge @L_8004F794
    li r0, -0xc8
    stw r0, 0xc(r1)
    @L_8004F794:
    lwz r7, 0xc(r1)
    li r3, 0x0
    li r4, 0x42
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F7AC:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F7B4:
    li r3, 0x0
    li r4, 0x43
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8004F82C
    lwz r0, 0x8(r1)
    cmpwi r0, 0xc8
    ble @L_8004F800
    li r0, 0xc8
    stw r0, 0x8(r1)
    @L_8004F800:
    lwz r0, 0x8(r1)
    cmpwi r0, -0xc8
    bge @L_8004F814
    li r0, -0xc8
    stw r0, 0x8(r1)
    @L_8004F814:
    lwz r7, 0x8(r1)
    li r3, 0x0
    li r4, 0x43
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FAA58
    @L_8004F82C:
    bl fn_8001E200
    b @L_8004EE3C
    @L_8004F834:
    li r3, 0x8c
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1
    @L_8004F848:
    lwz r0, 0x64(r1)
    lwz r31, 0x5c(r1)
    lwz r30, 0x58(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
